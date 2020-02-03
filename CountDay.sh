#!/bin/bash
startyear=$1
continueyear=$2
day=0
flag=0
function leap() {
    nowyear=$1
if [ $[nowyear % 4] -eq 0 ] && [ $[nowyear % 100] -ne 0 ];then
    flag=1
elif  [ $[nowyear % 400] -eq 0 ];then
    flag=1
else
    flag=0
fi
echo $flag
}

count=`leap $startyear`
if [[ $count -eq 1 ]];then
    day=366
else
    day=365
fi

for ((i=0;i<$continueyear;i++));do
    count1=`leap $i`
    year=$[$startyear+$i]
    if [[ $year -eq 1 ]];then
        day=$[day+366]
    else
        day=$[day+365]
    fi
done
echo $day
