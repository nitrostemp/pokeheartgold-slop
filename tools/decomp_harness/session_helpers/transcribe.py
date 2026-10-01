import re, sys
# transcribe a thumb function from asm/*.s into MWCC inline-asm with explicit pc-relative literal loads + dcd pool
src, fn = sys.argv[1], sys.argv[2]
symmap = dict(a.split('=') for a in sys.argv[3:])  # label=replacement
lines = open(src).read().split('\n')
start = lines.index(f'\tthumb_func_start {fn}')
end = lines.index(f'\tthumb_func_end {fn}', start)
body = lines[start+2:end]
# compute addresses
addr = 0; out = []; labels = {}; items = []
for l in body:
    t = l.split(';')[0].rstrip()
    m = re.match(r'^(\w+):\s*(\.word\s+(\S+))?', t.strip())
    if re.match(r'^\w+:$', t.strip()):
        labels[t.strip()[:-1]] = addr; items.append(('label', t.strip()[:-1], addr)); continue
    m = re.match(r'^(\w+):\s*\.word\s+(\S+)', t.strip())
    if m:
        labels[m.group(1)] = addr; items.append(('word', m.group(2), addr)); addr += 4; continue
    s = t.strip()
    if not s: continue
    if s.startswith('.balign'):
        if addr % 4: items.append(('ins', 'nop', addr)); addr += 2
        continue
    size = 4 if re.match(r'^(bl|blx)\s', s) and not re.match(r'^blx\s+r\d', s) else 2
    items.append(('ins', s, addr)); addr += size
res = []
for kind, s, a in items:
    if kind == 'label':
        res.append(f'{s}:')
    elif kind == 'word':
        res.append(f'    dcd {symmap.get(s, s)}')
    else:
        m = re.match(r'^ldr\s+(r\d),\s*(_\w+)$', s)
        if m:
            off = labels[m.group(2)] - ((a + 4) & ~3)
            res.append(f'    ldr {m.group(1)}, [pc, #{off:#x}]')
        else:
            s = re.sub(r'\[(r\d)\]', r'[\1, #0]', s)
            res.append('    ' + s)
print('\n'.join(res))
