#!/bin/bash
file=${1:--}


while read sample; do
	echo "${last:-$sample} $sample"
	last=$sample
done < <(cat $file)
