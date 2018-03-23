#!/bin/bash
word=${1:-hello}
for x in samples/$word-*.raw; do bin/play.sh $x; n=${x##*-}; n=${n%%.*}; echo $n; say -v $n $n; done
