#!/bin/bash
SWD=$(dirname $0)
file=${1:--}
. $SWD/setenv.sh
[ ! -f $file.raw ] && sox -d $soxParams $file.raw
play $soxParams $file.raw
[ ! -f $file.txt ] && $SWD/bin2dec.sh $file.raw > $file.txt
[ ! -f $file.png ] && $SWD/plot.sh $file.txt $file.png
[ ! -f $file.pairs ] && $SWD/pairs.sh $file.txt > $file.pairs
[ ! -f $file.trend ] && $SWD/trend.sh $file.txt > $file.trend
[ ! -f $file.smooth ] && $SWD/smooth.sh 16 $file.txt > $file.smooth
#[ ! -f $file.smooth.raw ] && xxd -r -p $file.smooth > $file.smooth.raw
#[ ! -f $file.smooth.txt ] && hexdump -v -e '1/1 "%u " "\n"' $file.smooth.raw > $file.smooth.txt
#[ ! -f $file.smooth.png ] && gnuplot -p -e "set terminal pngcairo; set output '$file.smooth.png'; plot '$file.smooth.txt' pt 0 with lines"
#play $soxParams $file.smooth.raw
