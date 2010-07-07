MODULES
=======

All modules may receive first a variable number of unnamed parameters, and then
a variable number of named parameters. 

None of the modules is finished, but they are functional. As more functionality
is needed they are more developed. See DEVELOP_MODULES to see how to create new
modules.

Allow
-----

Allow(ip1,ip2...). It allows access to request that come from one of the given 
IPs. 

AuthPAM
-------

AuthPAM(pammodulename). Allows access using basic authentication, using the 
pam module marked. See AUTH doc file for more information.

Directory
---------

Exports a directory, with all its contents, but not listing them, so you can 
only access if you know the exact path of the file.

DirList
-------

Same as Directory, but also gives a pretty listing (if resources are configured,
if not, ugly listing).

Fake
----

Does nothing. Might be usefull or not. But sure its not dangerous.

Include
-------

Includes another config file right here.

Path / [...]
------------

[regexp]. Allows access to next modules if it fits the regular expresion of
the parameter.

Proxy
-----

Proxies the requesto to another server, marked as http://server:port/path. 
Current path will be appended to path.

Resources
---------

Should be configured at */resources/*, and gives access to some onion+ images 
and error files.

Server / [[...]]
----------------

[[sever_regexp]]. Allows access to servers matching the regular expressin. This
is used to create virtual hosts, but its not mandatory it is at first level.

StaticFile 
----------

StaticFile(file, code=error_code). Returns a static file from your filesystem,
if no code is given its a *200 OK*, if given it is that one.

Stats
-----

Some esasy stats about the server status, like how many clients have been 
created, whats the client peak.

