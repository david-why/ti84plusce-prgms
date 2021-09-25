#!/bin/bash
if [ -z "$1" ]; then
    echo 'usage: ./mkproj.sh name'
    exit 0
fi

if [ -e "$1" ]; then echo 'error: file/directory exists'; exit 1; fi
cp -r template "$1"
sed -i 's/# \[name\]/# '"$1"'/g' "$1"/README.md
sed -i 's/= name/= "'"${1^^}"'"/g' "$1"/makefile

