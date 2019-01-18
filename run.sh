#!/bin/bash

make

cd bin
mkdir cgi-bin
mv webinterface cgi-bin

./mellon ../MountPoint --user=miguel --email=email@gmail.com
python3 -m http.server --cgi
