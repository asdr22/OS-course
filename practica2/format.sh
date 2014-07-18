#!/bin/sh
#set -x

RES=`echo "$1" | grep -E '^ssoo_p2(_100[[:digit:]]{6}){1,2}.zip$'`

if [ ! -f "$1" ] ;then
	echo "File does not exist"
	exit
fi

if [ "$RES" = "" ] ;then
	echo "Incorrect file name/format"
	exit
fi

echo "Correct file name/format"

ZIP=$(basename "$1")
ZIP="${ZIP%.*}"
unzip -q $1 -d $ZIP
cd $ZIP

if [ ! -e minishell.c -o ! -e Makefile ]; then
	echo "One or more of the files were not found"
fi

make clean 2> /dev/null > /dev/null
make       2> /dev/null > /dev/null

if [ ! -e minishell ]; then
	echo "Compilation error"
else
	echo "Compilation OK"
fi

cd ..
rm -r $ZIP
exit


