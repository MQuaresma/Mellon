# MellonFS
MellonFS is a userspace file system implemented on top of the FUSE kernel module via 
[libfuse](https://github.com/libfuse/libfuse).
The file system aims to improve access control mechanisms by adding two factor authentication.
A list of allowed users, and respective emails, is kept on an encrypted file (using AES in CBC mode) 
and each time an open system call is issued, a code is randomly generated and sent to the email of
the user that issued the system call to authenticate the request.
