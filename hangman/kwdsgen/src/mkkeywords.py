#!/usr/bin/env python3

words = []
try:
    while True:
        words.append(input().strip().upper())
        if len(words[-1]) > 11:
            exit(f'Error: Input {words[-1]!r} is not valid!')
except EOFError:
    pass

with open('kwds.h', 'w') as f:
    print(f'const uint16_t kwdslength = {len(words)};', file=f)
    print('const char *keywords[] = {', file=f)
    for w in words:
        print(f'    "{w}",', file=f)
    print('};', file=f)
