What?
=====
__p__ rocess __a__ uto - __r__ estart - er.

Input
-----

1. a timeout
2. a command to run

Output
------
None

Side effect
-----------
Repeat

1. Fork and exec the given command.
2. Parent sleeps for _timeout_ amount of time.
3. Parent kills the child (if it is alive).

until parent receives a SIGTERM.

Install?
--------
	git clone https://github.com/arpankapoor/par.git
	make
	make install

How to use?
-----------
	par TIMEOUT COMMAND [OPTIONS]...

Why?
----
To make not-so-stable programs act like daemons.
