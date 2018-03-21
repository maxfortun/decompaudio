#!/bin/bash

file=$1
if [ ! -f "$file" ]; then
	echo "Usage: $0 <file>"
	exit
fi

xxd -r -p $file
