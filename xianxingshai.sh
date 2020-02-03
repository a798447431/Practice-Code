#!/bin/bash
num=0
sum=0
for((i=0; i<1000; i++)); do
    prime[${i}]=0
done

for((i=2; i<1000; i++)); do
    if [[ ${prime[$i]} -eq 0 ]];then
        prime[$num]=$i
        let num++
    fi
    for((j=0; j<${num}; j++));
    do
        pj=${prime[$j]}
        if [[ `expr ${i}\*$pj` -gt 1000 ]]; then
            break;
        fi
        prime[`expr ${i}\*${pj}`]=1
        if [[ `expr ${i}%${pj}` == 0 ]]; then
            break;
        fi
    done
done

for((i=0;i<$num;i++)); do
    echo ${prime[$i]}
    let sum+=${prime[$i]}
done
echo $sum
