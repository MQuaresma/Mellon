#!/usr/bin/env python
import os
from subprocess import Popen
from flask import Flask, request, render_template, redirect, url_for

APP = Flask(__name__)
os.mkfifo("mellon_fifo")

mellon_fifo = None

@APP.route('/', methods=['POST', 'GET'])
def login():
    if request.method == 'GET':
        return render_template('usr_cred.html')
    else:
        init_fs_daemon()
        return redirect(url_for('auth'))

def init_fs_daemon():
    global mellon_fifo
    uname = request.form['username']
    email = request.form['email']
    mellon_fs = Popen(['./bin/mellon', '../MountPoint', '--user='+uname, '--email='+email])
    mellon_fifo = open("mellon_fifo", "w")

@APP.route('/auth', methods=['GET', 'POST'])
def auth():
    if request.method == 'POST':
        code = request.form['fa_code']
        mellon_fifo.write(code[:5])
        mellon_fifo.flush()
    return render_template('authin.html')

def main():
    APP.run()

if __name__=="__main__":
    main()
