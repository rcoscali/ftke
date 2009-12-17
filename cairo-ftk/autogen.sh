#!/bin/bash

aclocal
automake -a
libtoolize --force --copy
autoconf

