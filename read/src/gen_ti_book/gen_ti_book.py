#!/usr/bin/python
# usage: gen_ti_book.py text_book ti_book [memlimit]

import os
from PIL import Image, ImageDraw, ImageFont

row = 12
col = 40

if len(os.sys.argv) < 3:
    text_book = 'demo.txt'
    ti_book = 'demo'
else:
    text_book = os.sys.argv[1]
    ti_book = os.sys.argv[2]

if len(os.sys.argv) < 4:
    memlimit = 0
else:
    memlimit = int(os.sys.argv[3])

book = open(text_book, 'rb').read()

try:
    text = book.decode('utf8')
except:
    try:
        text = book.decode('gbk')
    except:
        try:
            text = book.decode('ascii')
        except:
            print('error: failed to detect encoding.')
            os.sys.exit(1)

lines = []
i = 0
s = ''
t = 0
for c in text:
    if t >= col:
        lines.append(s)
        s = ''
        t = 0
    if c == '\n':
        lines.append(s)
        s = ''
        t = 0
        continue
    if c == '\t':
        t1 = (t + 3) // 4 * 4 - t
        s += ' ' * t1
        t += t1
        continue
    if c < ' ':
        continue
    if c > '~' and t >= col - 1:
        lines.append(s)
        s = ''
        t = 0
    s += c;
    t += 2 if c > '~' else 1
if s:
    lines.append(s)

pages = (len(lines) + row - 1) // row
while len(lines) < pages * row:
    lines.append('')

si = 1    
while lines:

    se = {}
    sc = {}

    sp = 0
    for a in lines:
        for c in a:
            if c > ' ':
                if c > '~':
                    sc[c] = 0
                else:
                    se[c] = 0
        sp += 1
        if memlimit > 0 and sp % row == 0 and 9 + (len(se) + len(sc) * 2) * 16 + sp * col >= memlimit:
            break

    sy = {}
    i = 1
    for c in sorted(se):
        se[c] = i
        sy[i] = c
        i += 1
    for c in sorted(sc):
        sc[c] = i
        sy[i] = c
        i += 2
    sym = i - 1

    print('book', text_book, 'size', len(text), 'symbols', len(se), '+', len(sc), '=', sym, 'lines', sp, '(', si, ') pages', sp // row)

    font = ImageFont.truetype('font.ttf', 16)
    ti = open(ti_book + '.' + ('tib' if memlimit == 0 else ('%03d' % si)), 'wb')
    si += 1

    ti.write(bytes([16, 8, 3, row, col, sym // 256, sym % 256, sp // row // 256, (sp // row) % 256]))

    im = Image.new('1', (16, 16), 0)
    draw = ImageDraw.Draw(im)
    for i in sorted(sy):
        c = sy[i]
        draw.rectangle((0, 0, 16, 16), 0, 0)
        draw.text((0, 0), c, font=font, fill=1)
        d = list(im.getdata())
        for j in range(16):
            t = 0
            for k in range(8):
                t = t + t + d[j * 16 + k]
            ti.write(bytes([t]))
        if c in sc:
            for j in range(16):
                t = 0
                for k in range(8):
                    t = t + t + d[j * 16 + k + 8]
                ti.write(bytes([t]))

        
    se[' '] = 0
    for i in range(sp):
        s = lines[i]
        t = 0
        for c in s:
            if c in se:
                ti.write(bytes([se[c]]))
                t += 1
            else:
                ti.write(bytes([sc[c] // 256 + 128, sc[c] % 256]))
                t += 2
        if t < col:
                ti.write(bytes([0] * (col - t)))
    lines = lines[sp:]
        
    ti.close()

