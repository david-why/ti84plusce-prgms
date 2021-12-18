import os
import shutil
import subprocess
import zipfile
from io import BytesIO
from pathlib import Path
from tempfile import mkdtemp

from flask import *

DIR = os.path.dirname(os.path.abspath(__file__))

app = Flask(__name__)


def gen_archive(path):
    ls = os.listdir(path)
    z = zipfile.ZipFile(path / 'files.zip', 'w')
    for x in ls:
        if x.lower().endswith(('8xp', '8xv', 'tib')) or x[-3:].isdigit():
            z.write(path / x, x)
    z.close()
    with open(path / 'files.zip', 'rb') as f:
        return f.read()


@app.route('/', methods=['GET', 'POST'])
def home():
    if request.method == 'GET':
        return render_template('gen.html')
    typ = request.values.get('t')
    f = request.files.get('f')
    if f is None:
        return abort(400)
    buf = BytesIO()
    f.save(buf)
    s = buf.getvalue()
    buf.close()
    if typ == '1':
        td = mkdtemp()
        shutil.copytree(os.path.join(DIR, 'files'), os.path.join(td, 'files'))
        files = Path(td, 'files')
        with open(files / 'file.txt', 'wb') as f:
            f.write(s)
        p = subprocess.run(
            [
                'python3',
                'gen_ti_book.py',
                'file.txt',
                os.path.join('src', 'data'),
            ],
            capture_output=True,
            cwd=files,
        )
        if p.returncode != 0:
            shutil.rmtree(td)
            return (
                b'<h1>Error generating TIB</h1><pre><code>'
                + p.stderr
                + b'</code></pre>',
                500,
            )
        with open(files / 'src' / 'data.tib', 'rb') as fin:
            with open(files / 'src' / 'data.h', 'w') as fout:
                while data := fin.read(1):
                    fout.write(str(ord(data)) + ',')
        p = subprocess.run(['make'], capture_output=True, cwd=files)
        if p.returncode != 0:
            shutil.rmtree(td)
            return (
                b'<h1>Error generating 8XP</h1><pre><code>'
                + p.stderr
                + b'</code></pre>',
                500,
            )
        if 'bin' not in os.listdir(files):
            shutil.rmtree(td)
            return (
                '<h1>Error generating 8XP</h1><p>BIN directory not found!</p>'
            )
        data = gen_archive(files / 'bin')
        shutil.rmtree(td)
        return send_file(
            BytesIO(data),
            'application/zip',
            True,
            download_name='book.zip',
        )
    elif typ == '2':
        n = request.values.get('n')
        if n is None:
            return abort(400)
        td = mkdtemp()
        shutil.copytree(os.path.join(DIR, 'files'), os.path.join(td, 'files'))
        files = Path(td, 'files')
        with open(files / 'file.txt', 'wb') as f:
            f.write(s)
        with open(files / 'src' / 'main.cpp') as f:
            data = f.read()
        with open(files / 'src' / 'main.cpp', 'w') as f:
            f.write(data.replace('book_bin.h', 'book_fat_files.h'))
        env = os.environ.copy()
        env['CEDEV'] = env['USB_CEDEV']
        env['PATH'] = os.pathsep.join(
            env['PATH'].split(os.pathsep) + [os.path.join(env['CEDEV'], 'bin')]
        )
        p = subprocess.run(['make'], capture_output=True, cwd=files, env=env)
        if p.returncode != 0:
            shutil.rmtree(td)
            return (
                b'<h1>Error generating 8XP</h1><pre><code>'
                + p.stderr
                + b'</code></pre>',
                500,
            )
        p = subprocess.run(
            [
                'python3',
                'gen_ti_book.py',
                'file.txt',
                os.path.join('bin', 'book'),
                n,
            ],
            capture_output=True,
            cwd=files,
        )
        if p.returncode != 0:
            shutil.rmtree(td)
            return (
                b'<h1>Error generating parts</h1><pre><code>'
                + p.stderr
                + b'</code></pre>',
                500,
            )
        if 'bin' not in os.listdir(files):
            shutil.rmtree(td)
            return (
                '<h1>Error generating 8XP</h1><p>BIN directory not found!</p>',
                500,
            )
        data = gen_archive(files / 'bin')
        shutil.rmtree(td)
        return send_file(
            BytesIO(data),
            'application/zip',
            True,
            download_name='book.zip',
        )
    else:
        return abort(500)


if __name__ == '__main__':
    app.run('0.0.0.0', 5000, debug=True)
