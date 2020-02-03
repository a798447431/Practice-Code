#!/bin/bash

argc_num=$#

function parm() {
    if [[ $argc -gt 1 ]]; then
       argv=`echo "$*" | cut -d " " -f 2-`
    fi
    echo ${argv}
}

function is_c() {
    c_file=$1
    gcc $c_file -lm && time ./a.out `parm` && rm -f a.out
}

function is_c++() {
    cpp_file=$1
    g++ $cpp_file && time ./a.out `parm` && rm -f a.out
}

function is_bash() {
    sh_file=$1
    time bash $sh_file `parm`
}

if [[ $1 == *.c ]]; then 
    is_c $1 
elif [[ $1 == *.cpp ]]; then
    is_c++ $1
elif [[ $1 == *.sh ]]; then
    is_bash $1
else 
    echo "wrong file!"
fi


