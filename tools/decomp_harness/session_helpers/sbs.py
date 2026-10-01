import subprocess, sys, re, itertools
name, fn = sys.argv[1], sys.argv[2]
A=f'build/heartgold.us/asm/{name}.o'; C=f'build/heartgold.us/compile_one/{name}.o'
out=subprocess.run(['python3','tools/decomp_harness/objdiff.py',A,C,'--disasm',fn],capture_output=True,text=True,cwd=__import__('os').path.abspath(__import__('os').path.join(__import__('os').path.dirname(__file__), '../../..'))).stdout
parts=out.split('=== C:')
def norm(block):
    res=[]; base=None
    for l in block.split('\n'):
        m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{4}(?: [0-9a-f]{4})?)\s+(.*)',l)
        if m:
            addr=int(m.group(1),16)
            if base is None: base=addr
            ins=re.sub(r'\s*@.*','',m.group(3))
            ins=re.sub(r'<[^>]*>','',ins).strip()
            mm=re.match(r'^(b\w*(?:\.n|\.w)?)\s+([0-9a-f]+)$',ins)
            if mm:
                if mm.group(1).startswith('bl'): ins=mm.group(1)
                else: ins=f'{mm.group(1)} +{int(mm.group(2),16)-base:#x}'
            if m.group(2).startswith('0000') or '.word' in ins: ins='.word'
            res.append((addr-base, ins))
    return res
a=norm(parts[0]); c=norm(parts[1]) if len(parts)>1 else []
for x,y in itertools.zip_longest(a,c,fillvalue=(0,'')):
    mark='  ' if x[1]==y[1] else '!!'
    print(f'{x[0]:4x} {mark} {x[1]:38s} | {y[1]}')
