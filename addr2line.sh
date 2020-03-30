#!/bin/sh
if [ $# != 3 ]; then
    echo 'Usage: addr2line.sh executefile addressfile functionfile'
    exit
fi

depth=0

cat $2 | while read line; do
    if [ "$line" = 'Enter' ]; then
        read line1
        read line2

        str0=$(printf "%${depth}s" | sed "s/ /-/g")
        depth=$(($depth + 4))
        str1=$(printf "%${depth}s" | sed "s/ /-/g")

        addr2line -e $1 -f $line1 -s | c++filt | sed 's/^/'$(echo "$str0")'/;s/\(.*\)-\(.*\)/\1 \2/' >>$3
        echo "$str1 call" | sed 's/\(.*\)----\(.*\)/\1>>>\2/' >>$3
        addr2line -e $1 -f $line2 -s | c++filt | sed 's/^/'$(echo "$str1")'/;s/\(.*\)-\(.*\)/\1 \2/' >>$3
        echo >>$3
    elif [ "$line" = 'Exit' ]; then
        read line1
        read line2

        str1=$(printf "%${depth}s" | sed "s/ /-/g")
        depth=$(($depth - 4))
        str0=$(printf "%${depth}s" | sed "s/ /-/g")

        addr2line -e $1 -f $line2 -s | c++filt | sed 's/^/'$(echo "$str1")'/;s/\(.*\)-\(.*\)/\1 \2/' >>$3
        echo "$str1 return" | sed 's/\(.*\)----\(.*\)/\1<<<\2/' >>$3
        addr2line -e $1 -f $line1 -s | c++filt | sed 's/^/'$(echo "$str0")'/;s/\(.*\)-\(.*\)/\1 \2/' >>$3
        echo >>$3
    fi
done
