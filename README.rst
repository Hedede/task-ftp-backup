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

Building
--------

Prerequisites
^^^^^^^^^^^^^

The code was tested with the following dependencies. It might work with earlier versions, however, that is not guaranteed.

* Visual Studio 2022
* SQLite 3.39.4 (included in the Visual Studio solution)
* GTest 1.8.1 (included as a NuGet package)
