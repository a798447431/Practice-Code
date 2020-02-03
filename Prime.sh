#!/bin/bash
declare -a prime
sum=0
function init() {
    for i in `seq 1 1000`;do
        eval $1[$i]=0
    done
}

init prime

for ((i=2; i<1000; i++)); do
    if [[ ${prime[$i]} -eq 0 ]]; then
        prime[${prime[0]}]=$i
        ((prime[0]++))
    else
        continue
    fi
    for ((j=1; j<${prime[0]}; j++)); do
        if [ $[ ${prime[$j]} * $i ] -gt 1000 ]; then
            break
        fi
        prime[$[ $i * ${prime[$j]} ]]=1
        if [ $[ $i % ${prime[$j]} ] -eq 0 ]; then
            break;
        fi
    done
done
echo ${prime[5]}
echo $sum
