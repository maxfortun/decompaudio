#!/bin/bash
fileIn=$1
fileOut=${2:-$1.png}
if [ ! -f "$fileIn" ]; then
	echo "Usage: $0 <file>"
	exit
fi
gnuplot -p -e "set terminal pngcairo; set output '$fileOut'; plot '$fileIn' pt 0 with lines"
