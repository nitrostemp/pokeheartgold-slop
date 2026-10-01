import sys, re
# show.py file.c asm.s fn [--asm|--c]
cfile, sfile, fn = sys.argv[1:4]
mode = sys.argv[4] if len(sys.argv) > 4 else 'both'
if mode in ('both', '--asm'):
    s = open(sfile).read()
    m = re.search(r'\tthumb_func_start ' + fn + r'\n(.*?)\tthumb_func_end ' + fn + r'\n', s, re.S) or re.search(r'\tarm_func_start ' + fn + r'\n(.*?)\tarm_func_end ' + fn + r'\n', s, re.S)
    print(m.group(1) if m else 'ASM NOT FOUND')
if mode in ('both', '--c'):
    c = open(cfile).read()
    m = re.search(r'^(?![ \t])[^\n;]*\b' + fn + r'\([^;{]*\)\s*\{', c, re.M)
    if not m:
        print('C NOT FOUND')
    else:
        i = m.start(); depth = 0; j = c.index('{', m.start())
        k = j
        while True:
            if c[k] == '{': depth += 1
            elif c[k] == '}':
                depth -= 1
                if depth == 0: break
            k += 1
        print(c[i:k+1])
