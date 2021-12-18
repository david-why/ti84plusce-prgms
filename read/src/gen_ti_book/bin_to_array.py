with open('demo.001', 'rb') as f:
    print(repr(list(f.read()))[1:-1])
