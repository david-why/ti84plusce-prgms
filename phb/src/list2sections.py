data = []
try:
    while True:
        data.append(input())
except EOFError:
    pass

name = data.pop(0)

out = []
lvl = 0
last = False
for i in range(len(data)):
    if not data[i]:
        if lvl:
            if last:
                out.pop()
            out.append(
                '<b>'
                + str(lvl)
                + (
                    'st'
                    if lvl == 1
                    else ('nd' if lvl == 2 else ('rd' if lvl == 3 else 'th'))
                )
                + ' Level</b>'
            )
        else:
            out.append('<b>Cantrips (0 level)</b>')
        last = True
        lvl += 1
    else:
        out.append(data[i])
        last = False

p = 1
for i in range(0, len(out), 20):
    print(
        '<section><h1>'
        + name
        + ' P'
        + str(p)
        + '</h1><p>'
        + '</p><br/><p>'.join(out[i : i + 20])
        + '</p></section>'
    )
    p += 1
