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
    if uname:
        uname = '--user='+uname
    email = request.form['email']
    if email:
        email = '--email='+email
    key = request.form['reg_key']
    if key:
        key = "--key_reg="+key
    mellon_fs = Popen(['./bin/mellon', '../MountPoint', uname, email, key])
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
