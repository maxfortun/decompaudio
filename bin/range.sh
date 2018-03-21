#!/bin/bash

file=$1
if [ ! -f "$file" ]; then
	echo "Usage: $0 <file>"
	exit
fi

min=0
max=0
while read sample; do
	if [ "$sample" -gt "$max" ]; then
		max=$sample
	elif [ "$sample" -lt "$min" ]; then
		min=$sample
	fi
done < <(cat $file)
echo $min $max


