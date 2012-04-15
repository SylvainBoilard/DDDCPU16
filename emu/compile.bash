#!/bin/bash

CC="gcc"
FLAGS="-ansi -pedantic"

if [ "$1" = "debug" ] ; then
    FLAGS="$FLAGS -pg"
else
    FLAGS="$FLAGS -s -O2"
fi

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
