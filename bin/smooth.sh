#!/bin/bash
size=$1
file=${2:--}

if [ -z "$1" ]; then
	echo "Usage: $0 <size> <file>"
	exit
fi

declare -a samples
sampledSize=0
index=0
sum=0
sumDiffSq=0
mean=0
variance=0
deviation=0

while read sample; do
	if [ "$sampledSize" == "$size" ]; then
		oldSample=${samples[$index]};
		#echo "old $index $oldSample"
		oldSampleDelta=$(( $oldSample - $mean ))
		oldSampleDiffSq=$(( oldSampleDelta * oldSampleDelta ))
		sum=$(( sum - oldSample ))
		sumDiffSq=$(( sumDiffSq - oldSampleDiffSq ))
		sumDiffSq=${sumDiffSq#-}
	fi

	if [ "$sampledSize" -lt "$size" ]; then
		sampledSize=$(( sampledSize + 1 ))
	fi

	sum=$(( sum + sample ))
	mean=$(( sum / sampledSize ))

	#echo $sample $sumDiffSq
	sampleDelta=$(( sample - mean ))
	sampleDiffSq=$(( sampleDelta * sampleDelta ))
	sumDiffSq=$(( sumDiffSq + sampleDiffSq ))

	variance=$(( sumDiffSq / sampledSize ))
	deviation=$( echo "sqrt($variance)"|bc)

	if [ -z "$deviation" ]; then
		echo "bad variance $sumDiffSq $sampledSize sqrt($variance)" >&2
		exit
	fi

	samples[$index]=$sample
	index=$(( index + 1 ))

	if [ "$index" == "$size" ]; then
      index=0
	fi

	echo "$sample $mean $variance $deviation"

done < <(cat $file)




