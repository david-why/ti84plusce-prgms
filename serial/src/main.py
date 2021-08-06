#!/usr/bin/env python3

# This file should be run on the computer that the TI-84 is
# connected with.

import fcntl
import os
import select
import subprocess
import time

import requests
from bs4 import BeautifulSoup


class Special:
    SPECIALS = set()

    def __init__(self, name, second='', alpha='', call=lambda *args, **kwargs: None):
        self.name = name
        self.second = second
        self.alpha = alpha
        self.call = call
        self.SPECIALS.add(self)

    def __call__(self, *args, **kwargs):
        return self.call(*args, **kwargs)

    def __str__(self):
        if second:
            return self.second if self.second else self.name
        if alpha:
            return self.alpha if self.alpha else self.name
        return self.name

    def __hash__(self):
        return hash(self.name) + hash(self.second) + hash(self.alpha) + hash(self.call)

    def __repr__(self):
        return f'Special(name={self.name!r}, second={self.second!r}, alpha={self.alpha!r}, call={self.call!r})'


second = False
# 0: none
# 1: alpha, lower
# 2: alpha, upper
alpha = 0


def alpha_stat():
    return alpha


def toggle_alpha():
    global alpha
    alpha += 1
    if alpha == 3:
        alpha = 0


def _call_alpha(*args, **kwargs):
    global second, alpha, path
    if second:
        if len(path) > 1:
            path = path[:-1]
            show_page()
        second = False
        alpha = 0
    else:
        toggle_alpha()


# region keycodes
# define sk_Down             0x01
# define sk_Left             0x02
# define sk_Right            0x03
# define sk_Up               0x04
# define sk_Enter            0x09
# define sk_2nd              0x36
# define sk_Clear            0x0F
# define sk_Alpha            0x30
# define sk_Add              0x0A  // Change to 0xF1
# define sk_Sub              0x0B
# define sk_Mul              0x0C
# define sk_Div              0x0D  // Change to 0xF0
# define sk_Graph            0x31
# define sk_Trace            0x32
# define sk_Zoom             0x33
# define sk_Window           0x34
# define sk_Yequ             0x35
# define sk_Mode             0x37
# define sk_Del              0x38
# define sk_Store            0x2A
# define sk_Ln               0x2B
# define sk_Log              0x2C
# define sk_Square           0x2D
# define sk_Recip            0x2E
# define sk_Math             0x2F
# define sk_0                0x21
# define sk_1                0x22
# define sk_4                0x23
# define sk_7                0x24
# define sk_2                0x1A
# define sk_5                0x1B
# define sk_8                0x1C
# define sk_3                0x12
# define sk_6                0x13
# define sk_9                0x14
# define sk_Comma            0x25
# define sk_Sin              0x26
# define sk_Apps             0x27
# define sk_GraphVar         0x28
# define sk_DecPnt           0x19
# define sk_LParen           0x1D
# define sk_Cos              0x1E
# define sk_Prgm             0x1F
# define sk_Stat             0x20
# define sk_Chs              0x11
# define sk_RParen           0x15
# define sk_Tan              0x16
# define sk_Vars             0x17
# define sk_Power            0x0E
# endregion keycodes

DECODE = {
    0x01: '↓',
    0x02: '←',
    0x03: '→',
    0x04: '↑',
    0x09: '\n',
    0x36: Special(
        '2nd', call=lambda *args, **kwargs: globals().__setitem__('second', not second)
    ),
    # 0x0f: Special('Clear'),  # Captured on calc
    0x30: Special('Alpha', 'A-lock', call=_call_alpha),
    # 0x0A: '+',  # Changed to ...
    0x51: '+',  # ... this, because LF gets turned into CR
    0x0B: '-',
    0x0C: '*',
    # 0x0D: '/',  # Changed to ...
    0x50: '/',  # ... this, because CR gets turned into LF
    0x31: 'graph',
    0x32: 'trace',
    0x33: 'zoom',
    0x34: 'window',
    0x35: 'y=',
    0x37: 'mode',
    0x38: 'del',
    0x2A: '=',
    0x2B: 'ln',
    0x2C: 'log',
    0x2D: '**2',
    0x2E: '**-1',
    0x2F: 'math',
    0x21: '0',
    0x22: '1',
    0x23: '4',
    0x24: '7',
    0x1A: '2',
    0x1B: '5',
    0x1C: '8',
    0x12: '3',
    0x13: '6',
    0x14: '9',
    0x25: ',',
    0x26: 'sin',
    0x27: 'apps',
    0x28: 'x',
    0x19: '.',
    0x1D: '(',
    0x1E: 'cos',
    0x1F: 'prgm',
    0x20: 'stat',
    0x11: '_',
    0x15: ')',
    0x16: 'tan',
    0x17: 'vars',
    0x0E: '^',
}
SECOND = {
    # 0x0A: 'mem',  # Changed to ...
    0x51: 'mem',  # ... this, because LF gets turned into CR
    0x0B: ']',
    0x0C: '[',
    # 0x0D: 'e',  # Changed to ...
    0x50: 'e',  # ... this, because CR gets turned into LF
    0x31: 'table',
    0x32: 'calc',
    0x33: 'format',
    0x34: 'tblset',
    0x35: 'stat plot',
    0x37: 'quit',
    0x38: 'ins',
    0x2A: 'rcl',
    0x2B: 'e^',
    0x2C: '10^',
    0x2D: '√',
    0x2E: 'matrix',
    0x2F: 'test',
    0x21: 'catalog',
    0x22: 'L1',
    0x23: 'L4',
    0x24: 'u',
    0x1A: 'L2',
    0x1B: 'L5',
    0x1C: 'v',
    0x12: 'L3',
    0x13: 'L6',
    0x14: 'w',
    0x25: 'EE',
    0x26: 'asin',
    0x27: 'angle',
    0x28: 'link',
    0x19: 'i',
    0x1D: '{',
    0x1E: 'acos',
    0x1F: 'draw',
    0x20: 'list',
    0x11: 'ans',
    0x15: '}',
    0x16: 'atan',
    0x17: 'distr',
    0x0E: 'π',
}
ALPHA = {
    # 0x0A: '"',  # Changed to ...
    0x51: '"',  # ... this, because LF gets turned into CR
    0x0B: 'w',
    0x0C: 'r',
    # 0x0D: 'm',  # Changed to ...
    0x50: 'm',  # ... this, because CR gets turned into LF
    0x31: 'f5',
    0x32: 'f4',
    0x33: 'f3',
    0x34: 'f2',
    0x35: 'f1',
    0x2A: 'x',
    0x2B: 's',
    0x2C: 'n',
    0x2D: 'i',
    0x2E: 'd',
    0x2F: 'a',
    0x21: ' ',
    0x22: 'y',
    0x23: 't',
    0x24: 'o',
    0x1A: 'z',
    0x1B: 'u',
    0x1C: 'p',
    0x12: '=',
    0x13: 'v',
    0x14: 'q',
    0x25: 'j',
    0x26: 'e',
    0x27: 'b',
    0x19: ':',
    0x1D: 'k',
    0x1E: 'f',
    0x1F: 'c',
    0x11: '?',
    0x15: 'l',
    0x16: 'g',
    0x0E: 'h',
}


def decode(c: int):
    if second:
        return SECOND.get(c, DECODE[c])
    if alpha == 1:
        return ALPHA.get(c, DECODE[c])
    if alpha == 2:
        if c == 0x19:  # (alpha)(alpha)(.) => ";"
            return ';'
        char = ALPHA.get(c, DECODE[c])
        if isinstance(char, Special):
            return char
        return char.upper()
    return DECODE[c]


pyterm_term = None


def pyterm(c=None):
    global pyterm_term, path
    if pyterm_term is None:
        pyterm_term = subprocess.Popen(
            ['python3', '-i'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        fcntl.fcntl(pyterm_term.stdin.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
        fcntl.fcntl(pyterm_term.stdout.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
        fcntl.fcntl(pyterm_term.stderr.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
    try:
        if c:
            pyterm_term.stdin.write(c.encode())
            pyterm_term.stdin.flush()
        time.sleep(0.5)
        o = pyterm_term.stdout.read()
        if o is None:
            o = b''
        e = pyterm_term.stderr.read()
        if e is None:
            e = b''
        wfile.write((c.encode() if c else b'') + o + e)
        wfile.flush()
    except BrokenPipeError:
        pyterm_term = None
        path = path[:-1]
        show_page()


codes_page = 0


def codes(c=None):
    global codes_page, path
    if c == 'del':
        codes_page = 0
        path = path[:-1]
        show_page()
        return
    if not c:
        codes_page = 0
    if codes_page == 0:
        wfile.write(
            b'\x02We use some special combi-nations for keys that are useful. '
            b'Here is the full  list of them:\n'
            b'*2nd to toggle blue key\n'
            b'*alpha to toggle original,lower, upper\n'
            b'*(alpha)(3) => "="\n'
            b'*(X,T,[,n) => "x"\n'
            b'{Any=More,DEL=Quit}'
        )
        wfile.flush()
    elif codes_page == 1:
        wfile.write(
            b'\x02*(2nd)(alpha) => prev page'
            b'*(alpha)(3) => "theta"\n'
            b'*(alpha*2)(.) => ";"\n'
            b'*(clear) => quit\n'
            b'*(neg) => "_"\n'
            b'*(alpha*2)(neg) => "!"\n'
            b'*(sto->) => "="\n'
            b'*(^-1) => "**-1"\n'
            b'*(^2) => "**2"\n'
            b'{Any=More,DEL=Quit}'
        )
        wfile.flush()
    elif codes_page == 2:
        wfile.write(
            b'\x02*(^) => "**"\n'
            b'*(alpha)(3) => "theta"\n'
            b'*(alpha*2)(.) => ";"\n'
            b'*(clear) => quit\n'
            b'*(neg) => "_"\n'
            b'*(alpha*2)(neg) => "!"\n'
            b'*(sto->) => "="\n'
            b'*(^-1) => "**-1"\n'
            b'*(^2) => **2\n'
            b'{Any=More,DEL=Quit}'
        )
        wfile.flush()
    elif codes_page == 3:
        codes_page = 0
        path = path[:-1]
        show_page()
        return
    codes_page += 1


browser_url = ''
browser_data = None


def browser(c=None):
    global browser_url, browser_data, path
    if c is None:
        browser_url = ''
        browser_data = None
        wfile.write(b'\x02Enter URL here:\n> ')
        wfile.flush()
        return
    if browser_data is None:
        if c == '\n':
            browser_data = BeautifulSoup(
                requests.get(browser_url).content, 'html.parser'
            )
            wfile.write(b'\n'.join(x.encode() for x in browser_data.strings if x.strip()))
        else:
            if c == 'del':
                if browser_url:
                    browser_url = browser_url[:-1]
                    wfile.write(b'\x08')
            else:
                browser_url += c
                wfile.write(c.encode())
        wfile.flush()
    else:
        print('qq')
        path = path[:-1]
        show_page()


PAGES = {
    'home': [
        ('Get Python terminal', 'pyterm'),
        ('See special codes', 'codes'),
        ('Use browser', 'browser'),
    ],
    'pyterm': pyterm,
    'codes': codes,
    'browser': browser,
}

path = ['home']


def show_page():
    pname = path[-1]
    page = PAGES[pname]
    if callable(page):
        page()
    else:
        wfile.write(b'\x02')
        wfile.write(pname.encode())
        wfile.write(b'\n')
        for i, (line, _) in enumerate(page):
            wfile.write(f'{i+1}: {line}\n'.encode())
        wfile.flush()


def handle_char(c: int):
    pname = path[-1]
    page = PAGES[pname]
    char = decode(c)
    if isinstance(char, Special):
        char()
        return
    if callable(page):
        page(char)
    else:
        if isinstance(char, str) and char in '123456789':
            index = int(char) - 1
            if index < 0 or index >= len(page):
                return
            wfile.write(f'{page[index][0]}\n'.encode())
            wfile.flush()
            path.append(page[index][1])
            show_page()
        elif char == '⁻':
            show_page()


while not os.path.exists('/dev/ttyACM0'):
    time.sleep(0.01)

rfile = open('/dev/ttyACM0', 'rb')
wfile = open('/dev/ttyACM0', 'wb')
show_page()

while os.path.exists('/dev/ttyACM0'):
    rfiles, wfiles, xfiles = select.select([rfile, os.sys.stdin], [], [], 1)
    if not os.path.exists('/dev/ttyACM0'):
        os.sys.exit()
    for file in rfiles:
        if file is rfile:
            data = rfile.read(1)
            handle_char(ord(data))
        elif file is os.sys.stdin:
            data = input()
            if data == '.q':
                wfile.write(b'\x01')
                wfile.flush()
                os.sys.exit()
            elif data == '.c':
                wfile.write(b'\x02')
            else:
                wfile.write((data + '\n').encode())
            wfile.flush()
