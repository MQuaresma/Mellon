#!/usr/bin/env python
import os
from subprocess import Popen, PIPE
from flask import Flask, request, render_template

app = Flask(__name__)
os.mkfifo("mellon_fifo")

mellon_fs = Popen(['./bin/mellon', '../MountPoint', '--user=miguel', '--email=miguelmirq@gmail.com'])
mellon_fifo = open("mellon_fifo", "w")

@app.route('/')
def auth():
    return render_template('authin.html')

@app.route('/', methods=['POST'])
def getCode():
    code = request.form['fa_code']
    mellon_fifo.write(code)
    mellon_fifo.flush()
    return render_template('authin.html')

def main():
    app.run()

if __name__=="__main__":
    main()
