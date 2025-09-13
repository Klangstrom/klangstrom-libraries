#!/bin/zsh

# TODO WIP this is macOS only for now

SRC="../libraries"
DST="$HOME/Documents/Arduino/libraries"
echo
echo "linking libraries from '$SRC' into '$DST'"
echo 
./link_libraries.sh klangstrom-libraries.config $SRC $DST
