#!/usr/bin/env python
from subprocess import Popen, PIPE
from flask import Flask, request, render_template
app = Flask(__name__)

mellon_fs = Popen(['./bin/mellon ../MountPoint --user=miguel --email=miguelmirq@gmail.com'], shell=True, stdin=PIPE)

@app.route('/')
def auth():
    return render_template('authin.html')

@app.route('/', methods=['POST'])
def getCode():
    code = request.form['fa_code']
    mellon_fs.communicate(input=code)
    return render_template('authin.html')

def main():
    app.run()

if __name__ == "__main__":
    main()
