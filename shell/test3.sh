#!/bin/bash 
function time_h() {
    now_hour=`date +%k`
    echo $now_hour
}

function time_m() {
    now_minute=`date +%M | sed s'/^0//'`
    echo $now_minute
}

function time_s() {
    now_second=`date +%S | sed s'/^0//'`
    echo $now_second
}

start_h=`time_h`
start_m=`time_m`
start_s=`time_s`

while [[ 1 -eq 1 ]]; do
    end_h=`time_h`
    end_m=`time_m`
    end_s=`time_s`
    if [[ $end_m -eq 0 ]]; then
        export DISPLAY=:0.0 && notify-send "现在时间 $end_h点整！"
    fi
    min_dif=$[$[end_h-start_h]*60+$[end_m-start_m]]
    if [[ $min_dif -eq 1 ]]; then
        export DISPLAY=:0.0 && notify-send "你已经工作４５分钟了，休息一下吧！"
    fi
    if [[ $min_dif -eq 2 ]]; then
        export DISPLAY=:0.0 && notify-send "你已经休息５分钟了，继续工作吧！"
        start_h=`time_h`
        start_m=`time_m`
        start_s=`time_s`
    fi
done

