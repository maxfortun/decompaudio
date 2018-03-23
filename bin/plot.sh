#!/bin/bash
fileIn=$1
fileOut=${2:-$1.png}
if [ ! -f "$fileIn" ]; then
	echo "Usage: $0 <file>"
	exit
fi
samples=$(wc -l $fileIn|awk '{ print $1 }')
gnuplot -p -e "set samples $samples; set terminal pngcairo; set output '$fileOut'; plot '$fileIn' pt 0 with lines"
