#!/bin/bash

CC="gcc"
FLAGS="-ansi -pedantic"

case "$1" in
    "debug")
	FLAGS="$FLAGS -g" ;;
    ""|"release")
	FLAGS="$FLAGS -s -O2" ;;
esac

LINK=1

for file in *.c ; do
    $CC -c $file $FLAGS
    if [ $? -ne 0 ] ; then
	echo $file : compilation failed.
	LINK=0
    fi
done

if [ $LINK -eq 1 ] ; then
    gcc *.o -o DCPU16
fi
