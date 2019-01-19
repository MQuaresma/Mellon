#!/usr/bin/env python
import os
import select
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
        if init_fs_daemon()==0:
            return redirect(url_for('auth'))
        else:
            return redirect(url_for('login'))

def init_fs_daemon():
    global mellon_fifo
    args=['./bin/mellon', '../MountPoint']
    uname = request.form['username']
    if uname:
        args.append('--user='+uname)
    email = request.form['email']
    if email:
        args.append('--email='+email)
    key = request.form['reg_key']
    if key:
        args.append('--master_key='+key)
    mellon_fs = Popen(args)
    mellon_fifo = open("mellon_fifo", "w")
    poller = select.poll()
    poller.register(mellon_fifo, select.POLLHUP)
    fd, mask = poller.poll(0)
    if fd == mellon_fifo.fileno() and mask & select.POLLHUP:        #Check SUCCESS
        mellon_fifo.close()
        return -1
    else:
        return 0

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
