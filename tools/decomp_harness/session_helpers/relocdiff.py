#!/usr/bin/env python3
"""Compare resolved relocation targets of an asm reference .o and a C .o.

usage: relocdiff.py <asm.o> <c.o>

objdiff.py masks relocated bytes, so a C file whose literal pools or
.rodata tables point at the wrong (but byte-identical) object still reports
MATCH, and only main.sbin catches it. This resolves every relocation in
.text/.rodata/.data to (function-relative offset, target) for R_ARM_ABS32 words (literal pools,
tables; assembler-resolved local `bl`s carry no relocation in the asm .o) where target is a
symbol name for functions/globals or `<section>+offset` for local data, then
diffs the two lists per function.
"""
import re
import subprocess
import sys


def run(*args):
    return subprocess.run(args, capture_output=True, text=True, check=True).stdout


def symbols(obj):
    """section index -> list of (value, size, name, type) and index -> name map."""
    out = run('arm-none-eabi-readelf', '-sW', obj)
    syms = []
    for line in out.splitlines():
        m = re.match(r'\s*\d+:\s+([0-9a-f]+)\s+(\d+)\s+(\w+)\s+(\w+)\s+\w+\s+(\w+)\s+(\S+)', line)
        if m:
            val, size, typ, bind, ndx, name = m.groups()
            syms.append((int(val, 16), int(size), typ, ndx, name))
    return syms


def sections(obj):
    out = run('arm-none-eabi-readelf', '-SW', obj)
    secs = {}
    for line in out.splitlines():
        m = re.match(r'\s*\[\s*(\d+)\]\s+(\S+)\s+\S+\s+[0-9a-f]+\s+([0-9a-f]+)\s+([0-9a-f]+)', line)
        if m:
            secs[int(m.group(1))] = (m.group(2), int(m.group(3), 16), int(m.group(4), 16))
    return secs


def relocs(obj):
    """Yield (target section index, offset, symbol name, addend)."""
    secs = sections(obj)
    syms = symbols(obj)
    out = run('arm-none-eabi-readelf', '-rW', obj)
    cur = None
    res = []
    for line in out.splitlines():
        m = re.match(r"Relocation section '\.rela?(\S+)' at offset (0x[0-9a-f]+)", line)
        if m:
            off = int(m.group(2), 16)
            cur = None
            for idx, (name, fo, sz) in secs.items():
                if fo == off and name.startswith('.rel'):
                    cur = idx
            continue
        m = re.match(r'\s*([0-9a-f]+)\s+[0-9a-f]+\s+(\S+)\s+[0-9a-f]+\s+(\S+)(?:\s*\+\s*([0-9a-f]+))?', line)
        if m and cur is not None:
            res.append((cur, int(m.group(1), 16), m.group(2), m.group(3), int(m.group(4) or '0', 16)))
    return secs, syms, res


def describe(obj):
    secs, syms, rels = relocs(obj)
    # map relocation section index -> target section (sh_info); approximate via
    # ordering: MWCC/as emit .rela.X immediately after X.
    byname = {n: (v, s, t, ndx) for v, s, t, ndx, n in syms}
    funcs = [(v & ~1, s, n, ndx) for v, s, t, ndx, n in syms if t == 'FUNC' and not n.startswith('$')]
    objs = [(v, s, n, ndx) for v, s, t, ndx, n in syms if t == 'OBJECT']
    entries = []
    for relsec, off, typ, sym, addend in rels:
        tsec = relsec - 1
        tname = secs.get(tsec, ('?',))[0]
        if typ != 'R_ARM_ABS32' or not tname.startswith(('.text', '.rodata', '.data', '.init', '.sdata')):
            continue
        owner = None
        best = -1
        for v, s, n, ndx in funcs:
            # Nearest preceding symbol: asm thumb_func symbols may carry no size.
            if ndx == str(tsec) and best < v <= off:
                best = v
                owner = (n, off - v)
        if owner is None:
            owner = (tname, off)
        # Resolve section/local-label targets to a data-relative description.
        target = sym
        info = byname.get(sym)
        if info is not None and info[2] != 'FUNC' and info[3] not in ('UND', 'ABS'):
            secname = secs.get(int(info[3]), ('?',))[0]
            dest = info[0] + addend
            target = f'{secname}+{dest:#x}'
            for v, sz, n, ndx in funcs:
                if ndx == info[3] and v == dest & ~1:
                    target = n
        elif addend:
            target = f'{sym}+{addend:#x}'
        entries.append((owner[0], owner[1], typ, target))
    return entries


def main():
    a, c = describe(sys.argv[1]), describe(sys.argv[2])
    def bucket(entries):
        d = {}
        for fn, off, typ, tgt in entries:
            d.setdefault(fn, []).append((off, typ, tgt))
        return {k: sorted(v) for k, v in d.items()}
    ba, bc = bucket(a), bucket(c)
    bad = 0
    for fn in sorted(set(ba) | set(bc)):
        if ba.get(fn) != bc.get(fn):
            bad += 1
            print(f'== {fn}')
            sa, sc = ba.get(fn, []), bc.get(fn, [])
            for i in range(max(len(sa), len(sc))):
                x = sa[i] if i < len(sa) else None
                y = sc[i] if i < len(sc) else None
                if x != y:
                    print(f'   asm {x}\n   c   {y}')
    print('reloc targets MATCH' if not bad else f'{bad} owners differ')


if __name__ == '__main__':
    main()
