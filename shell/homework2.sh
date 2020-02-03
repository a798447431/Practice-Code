#!/bin/bash
#对应最大长度
Max[0]=0
Max[1]=0
Max[2]=0
Max[3]=0
#对应字符串
str[0]=NULL
str[1]=NULL
str[2]=NULL
str[3]=NULL
#对应目录
dir[0]=NULL
dir[1]=NULL
dir[2]=NULL
dir[3]=NULL
#对应行号
line[0]=0
line[1]=0
line[2]=0
line[3]=0
#存储参数
declare -a argv
#判断是否包含do
is_con=0 

function Fliter() {
    FliterFile=$1
        file ${FliterFile} | grep -q -w "text"
    if [[ $? -eq 0 ]]; then
        echo 0
    else
        echo 1
    fi
}
#获取行号
function get_line() {
    sed -n -e "/$1/=" $2
}
#判断文件是否包含在之前的参数内
function pd_dir() {
    local dname=$1
    local fname=$2
    if [[ -d $dname ]]; then
        for i in `ls -a ${dname}`; do
            if [[ x"$i" == x"." ]]; then
                continue
            fi
            if [[ x"$i" == x".." ]]; then
                continue
            fi
            if [[ ${dname}/${i} == $fname ]]; then
                is_con=1
            else
                pd_dir ${dname}/${i} $fname
            fi
        done
    elif [[ -f $dname ]];then
        if [[ $dname -ef $fname ]];then
            is_con=1
        fi
    fi
}

function FindFile() {
    local File=$1
    num=$2
    bj=0
    for ((m=1;m<num;m++));do
        pd_dir ${argv[m]} $File
        if [[ is_con -eq 1 ]];then
            bj=1
            is_con=0
            break
        fi
    done
    if [[ bj -eq 0 ]]; then
    for i in `cat ${File} | tr -c -s "A-Za-z0-9" "\n"`; do
        for ((j=0;j<=2;j++)); do    
            if [[ ${#i} -gt ${Max[j]} ]]; then
                for ((k=2;k>=j;k--)); do
                    str[k+1]=${str[k]}
                    Max[k+1]=${Max[k]}
                    dir[k+1]=${dir[k]}
                    line[k+1]=${line[k]}
                done
                Max[j]=${#i}
                str[j]=$i
                dir[j]=$1
                line[j]=`get_line $i ${File}`
                break
            fi
        done 
    done
    fi
}

function FindDir() {
    local DirName=$1
    local num=$2
    for i in `ls -a ${DirName}`; do
        if [[ x"$i" == x"." ]]; then
            continue
        fi
        if [[ x"$i" == x".." ]]; then
            continue
        fi
        if [[ -d ${DirName}/${i} ]]; then
            FindDir ${DirName}/${i} $num
        elif [[ -f ${DirName}/${i} ]];then
            Check=`Fliter ${DirName}/${i}`
            if [[ ${Check} -eq 0 ]]; then
                FindFile ${DirName}/${i} $num
            fi
        fi
    done
}

index=1
for j in "$@"; do
    argv[index]=${j}
    if [[ -d $j ]];then
        FindDir $j $index
    elif [[ -f $j ]];then
        Check=`Fliter $j`
        if [[ ${Check} -eq 0 ]]; then
            FindFile $j $index
        fi
    else 
        index=0
        printf "参数不合法\n"
        break
    fi
    index=$[ index + 1 ]
done
if [[ index -ne 0 ]]; then
    printf "%d:%s:%s:%d\n" ${Max[0]} ${str[0]} ${dir[0]} ${line[0]}
    printf "%d:%s:%s:%d\n" ${Max[1]} ${str[1]} ${dir[1]} ${line[1]}
    printf "%d:%s:%s:%d\n" ${Max[2]} ${str[2]} ${dir[2]} ${line[2]}
fi
