#!/usr/bin/bash
gcc -o barM barM.c -O2 -s -lX11
sudo mv barM /usr/bin/
echo "Put this in your xinitrc file"
echo "==============="
echo "barM &"
echo "==============="