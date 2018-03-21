#!/bin/bash
file=$1
if [ ! -f "$file" ]; then
	echo "Usage: $0 <file>"
	exit
fi
gnuplot -p -e "set terminal pngcairo; set output '$file.png'; plot '$file' pt 0 with lines"
