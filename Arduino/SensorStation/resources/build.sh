#!/bin/bash

echo "Preparing resource files"
cp html/favicon.png .

tools/minify html/index.html > index.html
tools/minify html/update.html > update.html

echo "Creating C headers"
xxd -i favicon.png > html_favicon.h
xxd -i index.html > html_index.h
xxd -i update.html > html_update.h

echo "Removing temprary files"
rm favicon.png
rm index.html
rm update.html

