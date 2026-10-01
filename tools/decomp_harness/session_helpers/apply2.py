import sys, re
# apply2.py file.c patchfile: blocks '//@@ fn'. If fn is inside an #ifdef NONMATCHING block, replace the whole block.
cfile, pf = sys.argv[1:3]
c = open(cfile).read()
blocks = re.split(r'^//@@ (\w+)\n', open(pf).read(), flags=re.M)[1:]
for fn, body in zip(blocks[0::2], blocks[1::2]):
    body = body.strip('\n')
    m = re.search(r'^(?![ \t])[^\n;]*\b' + fn + r'\([^;{]*\)\s*\{', c, re.M)
    if not m: print('NOT FOUND', fn); continue
    # check for preceding #ifdef NONMATCHING
    pre = c.rfind('#ifdef NONMATCHING\n', 0, m.start())
    between = c[pre:m.start()] if pre >= 0 else None
    if pre >= 0 and between.count('\n') <= 2:
        end = c.index('#endif', m.start()); end = c.index('\n', end) + 1
        # also drop a preceding '// clang-format' / comment line? keep
        c = c[:pre] + body + '\n' + c[end:]
    else:
        i = m.start(); depth = 0; k = c.index('{', m.start())
        while True:
            if c[k] == '{': depth += 1
            elif c[k] == '}':
                depth -= 1
                if depth == 0: break
            k += 1
        c = c[:i] + body + c[k+1:]
open(cfile, 'w').write(c)
