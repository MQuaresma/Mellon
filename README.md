# MellonFS
MellonFS is a file system implemented on top of [libfuse](https://github.com/libfuse/libfuse).
This file system aims to improve access control mechanisms by adding 2FA.
Each file maintains a list of allowed users and a contact information to which an access code is sent 
each time a user wishes to open a given file.
