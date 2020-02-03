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
    if [[ end_s -eq 0 ]]; then
        export DISPLAY=:0.0 && notify-send "已经${end_h}点整了！"
    fi
    tmp=$[ ($end_h - $start_h)*60 + ($end_m - $start_m) ]
    
done
