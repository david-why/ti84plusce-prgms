from PIL import Image, ImageDraw, ImageFont

# font = ImageFont.load('wqy.pil')
c = '你'
font = ImageFont.truetype('./IBM3161.ttf', 16)
# font = ImageFont.truetype('./wqy-zenhei.ttc', 16, index=2)
im = Image.new('1', (16 * len(c), 16), 0)
draw = ImageDraw.Draw(im)
draw.text((0, 0), c, font=font, fill=1)
data = im.getdata()
lst = []
lst2 = []
for i in range(16):
    s = 0
    for j in range(8):
        s += data[i * 16 + j] * (1 << j)
    lst.append(s)
    s = 0
    for j in range(8):
        s += data[i * 16 + j + 8] * (1 << j)
    lst2.append(s)
print(lst, lst2, im.size)
im.save('test.png')
