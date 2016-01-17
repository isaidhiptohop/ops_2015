#!/bin/bash
for ((N=1; N <= 10; N++))
do
    for ((mp=0; mp <= 10; mp++))
    do
        for ((a=1; a <= 10; a++))
        do
            echo "$N 1000 $mp $a"
            ./test $N 1000 $mp $a
        done
    done
done
