# BCSockets
Black Codex minimal socket implementation in C++

[![Build Status](https://travis-ci.com/tbc-beren/BCSockets.svg?branch=master)](https://travis-ci.com/tbc-beren/BCSockets)

## Overview
- ***include***: BCSockets C++ interface
- ***test***: Unit tests
- ***tlsclient***: A simple Tls Client tool

## Dependencies
This project depends on GTest to build and run unit tests.

GTest will be automatically downloaded from ***cmake*** generation phase.

## How to build and run unit tests (Linux only)
From the repository root create an output directory and use ***cmake*** to build the project.

```
$ mkdir -p out/test
$ cd out/test
$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug
$ make
$ ./BCSocketsTest
```

To build with OpenSSL support enabled:

```
$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug -DBCSOCKETS_OPENSSL=yes
```

## Coverage
This project uses lcov in order to provide code coverage information.

Coverage test includes Unit Test and a TLS request to ***https://www.iana.org/index.html***. Therefore this test requires a working internet ocnnection.

For convenience, coverage test can be run just by using ***cmake***:

$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug -DBCSOCKETS_OPENSSL=yes -DBCSOCKETS_TESTCOVERAGE=1
$ make coverage

After executing this command, coverage information can be accessed in the ***cov-results*** sub directory (index.html).
