#!/bin/bash

file=$1
if [ ! -f "$file" ]; then
	echo "Usage: $0 <file>"
	exit
fi

hexdump -v -e '1/1 "%u " "\n"' $file
