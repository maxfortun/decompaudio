#!/bin/bash
SWD=$(dirname $0)
. $SWD/setenv.sh

word=${1}
path=${2:-$SWD/../samples}

if [ -z "$word" ]; then
	echo "Usage: $0 <word> [path]"
	exit
fi

voices=$(say -v '?'|grep en_|awk '{ print $1 }')

for voice in $voices; do
	[ ! -f "$path/$word-$voice.aiff" ] && say --channels=$channels -v $voice -o "$path/$word-$voice.aiff" $word
	[ ! -f "$path/$word-$voice.raw" ] && $SWD/aiff2raw.sh "$path/$word-$voice.aiff" "$path/$word-$voice.raw"
done
