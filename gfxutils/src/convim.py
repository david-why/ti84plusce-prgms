import argparse
import os
import struct

from PIL import Image

ap = argparse.ArgumentParser(description='convert images for gfxutils')

ap.add_argument('input', type=argparse.FileType('rb'), help='input image')
ap.add_argument('output', type=argparse.FileType('wb'), help='output file')
ap.add_argument(
    '-o',
    '--output-type',
    default='h',
    choices=('h', 'gfx'),
    help='output format, either c for C code or gfx for raw data '
    '(default "c")',
    metavar='type',
    dest='otype',
)
ap.add_argument(
    '-n',
    '--name',
    default='image',
    help='name of C variable, only makes sense when output type is h '
    '(default "image")',
    metavar='name',
    dest='name',
)

args = ap.parse_args()


def rgb565(rgb):
    r, g, b = rgb
    return ((r & 0xF8) << 8) + ((g & 0xFC) << 3) + (b >> 3)


data = bytes()

with args.input as f:
    with Image.open(f).convert('RGB') as im:
        data += struct.pack('<HH', im.width, im.height)
        for i in range(im.width):
            for j in range(im.height):
                data += struct.pack('<H', rgb565(im.getpixel((i, j))))

with args.output as f:
    if args.otype == 'gfx':
        f.write(data)
    elif args.otype == 'h':
        name = args.name.encode()
        f.write(b'const unsigned char ')
        f.write(name)
        f.write(b'_data[%i] = {\n' % len(data))
        for i in range(0, len(data), 16):
            f.write(b'    ')
            for b in data[i : i + 16]:
                f.write(b'0x%02X' % b)
                f.write(b', ')
            f.write(b'\n')
        f.write(b'};\n')
        with open(os.path.splitext(f.name)[0] + '.h', 'wb') as h:
            macro = b'_' + name.upper() + b'_H'
            h.write(b'#ifndef ')
            h.write(macro)
            h.write(b'\n#define ')
            h.write(macro)
            h.write(b'\nextern const unsigned char ')
            h.write(name)
            h.write(b'_data[%i];\n#define ' % len(data))
            h.write(name)
            h.write(b' ((gfxutils_image_t *)')
            h.write(name)
            h.write(b'_data)\n#endif\n')
