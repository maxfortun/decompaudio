#!/bin/bash
SWD=$(dirname $0)
file=${1:--}
params="-c 1 -r 8k -b 8 -L -e unsigned -t raw"
[ ! -f $file.raw ] && sox -d $params $file.raw
play $params $file.raw
[ ! -f $file.txt ] && hexdump -v -e '1/1 "%u " "\n"' $file.raw > $file.txt
[ ! -f $file.png ] && gnuplot -p -e "set terminal pngcairo; set output '$file.png'; plot '$file.txt' pt 0 with lines"
[ ! -f $file.pairs ] && $SWD/pairs.sh $file.txt > $file.pairs
[ ! -f $file.trend ] && $SWD/trend.sh $file.txt > $file.trend
#[ ! -f $file.smooth ] && $SWD/smooth.sh 16 $file.txt > $file.smooth
#[ ! -f $file.smooth.raw ] && xxd -r -p $file.smooth > $file.smooth.raw
#[ ! -f $file.smooth.txt ] && hexdump -v -e '1/1 "%u " "\n"' $file.smooth.raw > $file.smooth.txt
#[ ! -f $file.smooth.png ] && gnuplot -p -e "set terminal pngcairo; set output '$file.smooth.png'; plot '$file.smooth.txt' pt 0 with lines"
#play $params $file.smooth.raw
