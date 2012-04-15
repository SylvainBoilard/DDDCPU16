#!/bin/bash

CC="gcc"
FLAGS="-g -ansi -pedantic"

for fichier in *.c ; do
    $CC -c $fichier $FLAGS
    if [ $? != 0 ] ; then
	echo Échec lors de la compilation. Arrêt.
    fi
done
gcc *.o -o DCPU16
