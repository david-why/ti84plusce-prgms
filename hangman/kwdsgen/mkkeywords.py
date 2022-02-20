#!/usr/bin/env python3

import struct

WORDLENGTH = 12

words = []
pname = input().strip().upper()
if len(pname) > 8 or not pname.isalpha():
    exit(f'Pack name {pname!r} not valid!')
try:
    while True:
        s = input().strip().upper()
        if len(s) > WORDLENGTH - 1:
            print(f'WARNING: Input {s!r} is too long (>11 chars), skipped!')
        elif not s:
            break
        else:
            words.append(s)
except EOFError:
    pass

with open(f'{pname}.bin', 'wb') as f:
    f.write(struct.pack('<HH', 12345, len(words)))
    for w in words:
        f.write(w.encode())
        f.write(b'\0' * (WORDLENGTH - len(w)))
