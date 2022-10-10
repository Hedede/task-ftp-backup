ftp-backup
==========

Creates an SQL dump and uploads it to the specified location on an FTP server.

Usage::

    ftp-backup [OPTIONS...] path_to_db

      -H: host to connect to the FTP server (default localhost)
      -P: port to connect to the FTP server (default 21)
      -d: output directory in the FTP server (default /)
      -h: show help message
      -p: password
      -u: username (default guest)

Design
------

The repository is split into three parts

* lib - most of the low-level details are implemented here,
* app - high-level application logic,
* test - various automated tests.


The code is split into several modules:

* utility - various helper functions and things that didn't fit elsewhere (e.g. command-line parser),
* database - classes responsible for connecting to a database, generating and executing SQL queries,
* network - network connection utilities,
* ftp - simple FTP client implementation.

Further development
^^^^^^^^^^^^^^^^^^^

There are several areas to improve.

* First off, better error reporting. Need to add more exception types with better error messages.
* Unicode support on Windows.
* Add more test, including integration tests. Currently only GTest is used, but external
  tools (such as netcat) are required to properly test everything.
* Implement more FTP features. Handle more unexpected situations.
* More things marked as TODO in the code.

Building
--------

Prerequisites
^^^^^^^^^^^^^

The code was tested with the following dependencies. It might work with earlier versions, however, that is not guaranteed.

* Visual Studio 2022
* SQLite 3.39.4 (included in the Visual Studio solution)
* GTest 1.8.1 (included as a NuGet package)

Testing
-------

Test suite requires local FTP server running on a default port.
Guest (anonymous) connections must be allowed.
FTP server must be configured with a root in C:/ftp (Windows) or '/tmp/ftp' (Linux).
