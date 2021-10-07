import bs4
import sys

htmlfile = sys.argv[1]
with open(htmlfile) as file:
    soup = bs4.BeautifulSoup(file.read(), 'html.parser')

main = soup.select_one('main')
code = 'const page pages[] = {\n    {'
for x in main.select('section'):
    if x.name is None:
        continue
    title = x.select_one('h1')
    pars = x.select('p')
    text = ''
    for p in pars:
        for x in p:
            if isinstance(x, str):
                text += x
            elif x.name == 'b' or x.name == 'em':
                text += '\\001' + x.text + '\\002'
        text += '\\n'
    code += '"' + title.text.replace('"', '\\"') + '", "' + text.replace('"', '\\"') + '"},\n    {'
with open('pages_data.hpp', 'w') as file:
    print(code[:-7] + '\n};', file=file)
