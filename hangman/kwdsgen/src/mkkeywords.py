#!/usr/bin/env python3

words = []
pname = input().strip()
if len(pname) > 8 or not pname.isalpha():
    exit(f'Pack name {pname!r} not valid!')
try:
    while True:
        s = input().strip().upper()
        if len(s) > 11:
            print(f'WARNING: Input {s!r} is not valid, skipped!')
        elif not s:
            pass
        else:
            words.append(s)
except EOFError:
    pass

with open('kwds.h', 'w') as f:
    print(f'const char packname[] = "{pname}";', file=f)
    print(f'const uint16_t kwdslength = {len(words)};', file=f)
    print('const char *keywords[] = {', file=f)
    for w in words:
        print(f'    "{w}",', file=f)
    print('};', file=f)
