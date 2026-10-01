#!/usr/bin/env python3
"""Try source variants of one function and report its objdiff result for each.

usage: variants.py src/<tu>.c <function> < variants.json

variants.json is a list of variants; each variant is a list of [old, new]
string replacements (first occurrence only) applied to the pristine TU.
The TU is restored afterwards. Output per variant: diff count, SIZE line,
0 (= function matches) or ERR (= compile failed).
"""
import json
import os
import re
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../..'))
tu, fn = sys.argv[1], sys.argv[2]
path = os.path.join(ROOT, tu)
base = open(path).read()
variants = json.load(sys.stdin)
try:
    for i, v in enumerate(variants):
        s = base
        ok = True
        for old, new in v:
            if old not in s:
                ok = False
                break
            s = s.replace(old, new, 1)
        if not ok:
            print(i, 'NOMATCH', flush=True)
            continue
        open(path, 'w').write(s)
        out = subprocess.run(['tools/decomp_harness/compile_one.sh', tu], cwd=ROOT,
                             capture_output=True, text=True).stdout
        if 'functions' not in out:
            print(i, 'ERR', flush=True)
            continue
        m = re.search(re.escape(fn) + r' \((\d+) diffs', out)
        m2 = re.search(re.escape(fn) + r' \((\d+) vs \d+\)', out)
        print(i, m.group(1) if m else (m2.group(0) if m2 else 0), flush=True)
finally:
    open(path, 'w').write(base)
