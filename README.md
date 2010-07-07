The Onion+ Web Server
=====================

The Onion+ web server is a web server developed in C++ with extensibility in 
mind. It features an internal path mechanism that remembers an onion (ok, 
actually its a tree, like all other web servers, but the configuration enforces 
this layer over layer mechanisms).

A very simple configuration is like this:

	[['.*']] -> Directory('/var/www');

A slightly more complex:

	[['.*']] -> { 
		Allow('127.0.0.1') -> AuthPAM("login") -> { 
			['^/resources/'] -> Resources(); 
			DirList('/var/www');
		}
		StaticFile(':404.html', code=404);
	}

	# DANGEROUS! Gives access to all hard drive to the virtual host *allaccess*.
	[['allaccess']] -> Directory('/');

This gives local users (Allow) access to the following in the chain, to be 
authenticated using PAM, on login reign, and if successfull, directory resources
are the internal applciation resources, and all the rest is the directory
*/var/www*. If remote host is not at *127.0.0.1*, then next rule is to return 
the *error.html* resource with code *404*. The : for resources is from Qt.

The last line is highly discouraged. It creates the virtual host 'allaccess' 
that creates access to all the hard drive. But dont worry, actually as the
virtual host .* was created before, and it has a last resort rule that will 
always return data, *StaticFile*, it is not possible to get to *allaccess*.

There are many modules that can be used, and the idea is to make them very esasy
to develop. Current modules may not be finished, but they are functional.

See **doc/MODULES.md** for a list of modules and how they work.

Environment vars
----------------

Following environmental vars are usefull to know:

	* ONIONDIR -- Path to look for onion stuff. For example the compilation 
								directory.

	* ONION_DEBUG -- When set prints more debug information. Very usefull when 
									 developing.

Command line arguments
----------------------

config_file -- File read config from, instead of /etc/onion+.conf

Testing
-------

To get a fast test (and recompilation by the way), go to test, and fire:

	$ONIONDIR/test$ ./start.sh

Ideally this should launch automated tests, but they are not done yet.


