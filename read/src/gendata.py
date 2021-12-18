import argparse
from typing import BinaryIO, TextIO
from PIL import ImageDraw, Image, ImageFont

ap = argparse.ArgumentParser(description='convert data for reader')
ap.add_argument(
    'text',
    type=argparse.FileType('r'),
    help='.txt file to convert',
)
ap.add_argument(
    'datfile',
    type=argparse.FileType('wb'),
    help='output array file',
)
ap.add_argument(
    '--font',
    type=lambda s: ImageFont.truetype(s, 16),
    help='TrueType font file',
    default='wqy-zenhei.ttc',
)
args = ap.parse_args()

txt = args.text  # type: TextIO
dat = args.datfile  # type: BinaryIO
font = args.font  # type: ImageFont.FreeTypeFont

im16 = Image.new('1', (16, 16))
d16 = ImageDraw.Draw(im16)
im8 = Image.new('1', (8, 16))
d8 = ImageDraw.Draw(im8)


def clear(draw):
    # type: (ImageDraw.ImageDraw) -> None
    draw.rectangle(((0, 0), draw._image.size), 1, 1)


dat.write(b'\x10\x08\x04\x28\x0c')

symbols = []
pages = [[[]]]
r = c = 0

while dat := (txt.read(1)):
    if ord(dat) > ord('z'):
        d = d16
        w = 16
    else:
        d = d8
        w = 8
    clear(d)
    d.text((0, 0), dat, 0, font)
