#!/bin/bash
file=${1:--}

deltaAll=0

lastTrend=none
trendDelta=0

while read sample; do
	[ -z "$last" ] && last=$sample
	{ [ "$sample" -gt "$last" ] && trend=up; } || { [ "$sample" -lt "$last" ] && trend=down; } || trend=none
	delta=$(( sample - last ))
	allDelta=$(( allDelta + delta ))
	
	[ "$trend" = "$lastTrend" ] && trendDelta=$(( trendDelta + delta )) || trendDelta=$delta
	#echo "$last $sample $delta $trend $trendDelta $allDelta"
	echo $(( 128 + delta ))
	last=$sample
	lastTrend=$trend
done < <(cat $file)
