#!/bin/sh
##
##  Visopsys
##  Copyright (C) 1998-2020 J. Andrew McLaughlin
##
##  trim-whitespace.sh
##

find . -type f -exec sed -i -e 's/[ \t]\+$//g' {} \;

