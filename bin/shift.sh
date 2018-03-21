#!/bin/bash

file=$1
offset=$2

if [ ! -f "$file" ] || [ -z "$offset" ]; then
	echo "Usage: $0 <file> <offset>"
	exit
fi

while read sample; do
	echo $(( sample + offset ))
done < <(cat $file)


