#!/bin/sh
if [ ! "$1" ]; then
    echo 'Usage: ./rename.sh <name>'
    exit
fi
tmpdir=$(mktemp -d)
cp -r makefile src "$tmpdir"
sed -i -e "s/GCONSOLE/$1/" "$tmpdir"/makefile
cwd="$(pwd)"
# shellcheck disable=SC2164
cd "$tmpdir"
make
# shellcheck disable=SC2164
cd "$cwd"
cp "$tmpdir"/bin/* bin
rm -rf "$tmpdir"
