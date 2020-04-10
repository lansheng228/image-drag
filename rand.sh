#!/usr/bin/env bash

#generated_random_number_of_non-repetition

n=$1
a=($(for ((i = 0; i < $n; i++)); do echo "$i $RANDOM"; done | sort -k2n | cut -d" " -f1))
echo ${a[*]}
