#!/bin/bash
make clean
make

rm -rf MellonFS
mkdir MellonFS
cd MellonFS
PWD=$(pwd)
cd ..
echo "MellonFS will be mounted in " $PWD
echo "Press ENTER to start MellonFS..."
read -r PWD

chmod +x mellon.py
./mellon.py
