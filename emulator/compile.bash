#!/bin/bash

CC="gcc"
FLAGS="-ansi -pedantic -Wall -Wextra -Wmain -Wswitch-default -Wmissing-include-dirs -Wmissing-declarations -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls -Winit-self -Wshadow"

case "$1" in
    "debug")
	FLAGS="$FLAGS -g" ;;
    ""|"release")
	FLAGS="$FLAGS -s -O2" ;;
    *)
	echo "Unrecognized build type."
	exit 1
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
    if [ $? -ne 0 ] ; then
	exit 1
    fi
else
    exit 1
fi
