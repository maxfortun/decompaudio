#!/bin/bash
SWD=$(dirname $0)
. $SWD/setenv.sh
fileAiff=$1
fileRaw=$2

if [ -z "$fileRaw" ]; then
	echo "$0: <file.aiff> <file.raw>"
	exit
fi

sox $fileAiff $soxParams $fileRaw
