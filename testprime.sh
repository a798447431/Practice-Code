#!/bin/bash

if [[ "x${1}" == x ]];then 
    MAX=10
    else
        MAX=${1}
fi

num=0

for((i = 0; i < ${MAX}; i++));
do
    prime[${i}]=0
done

for((i = 2; i < ${MAX}; i++));
do 
    if [[ ${prime[$i]} -eq 0 ]];then
        prime[$num]=$i
        ((num++))
    fi
    for((j = 0; j < ${num}; j++));
    do
        pj=${prime[$j]}
        if [[ `expr ${i}\*$pj` -gt ${MAX} ]];then
            break;
        fi
        prime[`expr ${i}\*${pj}`]=1
        if [[ `expr ${i}%${pj}` == 0 ]];then
            break;
        fi
    done

done

for((i=0;i<$num;i++));
do

    echo ${prime[$i]}

done

