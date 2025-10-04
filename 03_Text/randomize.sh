#!/bin/bash

delay=${1:-0.05}
input=$(cat)
if [ -z "$input" ]; then
    echo "Cannot read input" >&2
    exit 1
fi

tput clear
tput cup 0 0

result=$(
    x=0
    echo "$input" | while IFS= read -r line; do
        y=0
        while [ $y -lt ${#line} ]; do
            char="${line:$y:1}"
            if [ "$char" != "" ]; then
                printf "%s\n" "$x:$y:$char"
            fi
            y=$((y+1))
        done 
        x=$((x+1))
    done
)

shuffled=$(echo "$result" | shuf)

echo "$shuffled" | while IFS=: read -r x y char; do 
    if [ -n "$x" ]; then
        tput cup $x $y
        echo -n "$char"
        sleep "$delay"
    fi
done

num_row=$(echo "$input" | wc -l)
tput cup $num_row 0
