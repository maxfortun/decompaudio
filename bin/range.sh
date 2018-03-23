#!/bin/bash
echo $@
if [ ! -f "$1" ]; then
	echo "Usage: $0 <file>"
	exit
fi

for file in $@; do
	min=0
	max=0
	while read sample; do
		if [ "$sample" -gt "$max" ]; then
			max=$sample
		elif [ "$sample" -lt "$min" ]; then
			min=$sample
		fi
	done < <(cat $file)
	echo $file $min $max
done

