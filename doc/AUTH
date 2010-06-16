AuthPAM
=======

The AuthPAM module allows to use the systems PAM installation, with all its features, to control the 
authentication of an area of the server.

Normally you can use normal "login" account, but its only allowed, on some systems, to root user, so
its not the best option. Also it features a sleep timeout when nothing happends, which can block
the Onion server for several seconds and be used as a DoS attack. 

So its strongly suggested, that on delicate systems, user tweak the pam configuration if it needs a
better than the standard "login" config. For example using libpam-pwdfile.

How it works
------------

It just sends the username to PAM and as answer to all the questions is answers the password. More 
complex auth mehotds are available, but user must create its own modules to do so.

Example auth config
-------------------

One example using pam_pwdfile::

   auth required pam_pwdfile.so pwdfile=/etc/onion/onion.passwd nodelay
   account    required	/lib/security/pam_pwdb.so

It should point to a simple file with pairs of "user:password" lines. Password must be created with 
the crypt function. htpasswd (from apache, I know...) should be ok for managing such files, and apache 
ones should be compatibles.

Another example, using system accounts::

   auth    required        pam_unix.so  nodelay
   auth    required        pam_warn.so 
   account required        pam_unix.so 
   account required        pam_warn.so 
   password        required        pam_deny.so 
   password        required        pam_warn.so 
   session required        pam_unix.so 
   session required        pam_warn.so

