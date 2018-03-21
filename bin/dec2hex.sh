#!/bin/bash

file=$1
if [ ! -f "$file" ]; then
	echo "Usage: $0 <file>"
	exit
fi

while read dec; do
	printf "%02x\n" $dec
done < <(cat $file)



