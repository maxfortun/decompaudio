#!/bin/bash

file=$1
if [ ! -f "$file" ]; then
	echo "Usage: $0 <file>"
	exit
fi

SWD=$(dirname $0)
. $SWD/setenv.sh

play $soxParams $file

