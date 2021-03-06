#!/bin/bash
declare -a Max
declare -a MaxString
declare -a Fileinfo
declare -a temp_argv

flag=0

Max[0]=0
Max[1]=0
Max[2]=0

MaxString[0]=0
MaxString[1]=0
MaxString[2]=0

Fileinfo[0]=0:0
Fileinfo[1]=0:0
Fileinfo[2]=0:0

temp_argv[0]=0
temp_argv[1]=0
temp_argv[2]=0

Filter_conf=./filter.conf

function Filter() {
	File_type=`basename $1 | tr [.] "\n" | tail -1`
	grep -w ${File_type}  ${Filter_conf}
	if [[ $? -eq 0 ]]; then
		file $1 | grep -q -w "text"
		if [[ $? -eq 0 ]]; then
			return 1
		else
			return 0
		fi
	else
		return 1
	fi
}

function if_include() {
    local DirName=$1
    local FileName=$2
    if [[ -d $DirName ]]; then
        for i in `ls -a ${DirName}`; do
            if [[ ${DirName}/${i} == $FileName ]]; then
                flag=1
            fi
        done
    elif [[ -f $DirName ]];then
        if [[ $DirName -ef $Filename ]];then
            flag=1
        fi
    fi
}

function FindFile() { 
    local File=$1  
    for ((n=0;n<=2;n++));do
        if_include ${temp_argv[n]} $File
        if [[ $flag -eq 1 ]];then
            have_include=1
            flag=0
            break
        fi
    done
    if [[ $have_include -ne 1 ]]; then
        for i in `cat ${File} | tr -c -s "A-Za-z" "\n"`; do
            for ((j=0; j<2; j++)); do
                if [[ ${#i} -gt ${Max[j]} ]]; then
                    for ((k=2;k>=j;k--)); do
                        Max[k+1]=${Max[k]}
                        MaxString[k+1]=${MaxString[k]}
                        Fileinfo[k+1]=${Fileinfo[k]}
                    done
                    Max[j]=${#i}
                    MaxString[j]=${i}
                    Fileinfo[j]=${File}:`grep -n ${i} $File | cut -d ':' -f 1 |head -n 1`
                    break
                fi
            done
        done 
    fi
}

function FindDir() {
    local DirName=$1
    for i in `ls -A ${DirName}`; do
        if [[ -d ${DirName}/${i} ]]; then
            FindDir ${DirName}/${i}
        elif [[ -f ${DirName}/${i} ]]; then
            Check=`Fliter ${DirName}/${i}`
            if [[ ${Check} -eq 0 ]]; then
                FindFile ${DirName}/${i}
            fi
        fi
    done
}

for i in $@; do
	if [[ ! -e  $i ]]; then
		exit 1
	fi
	Args+=($i)
done

for i in $@; do
    if [[  -d $i ]]; then
        FindDir $i 
    elif [[ -f $i ]]; then
        Check=`Fliter $i`
        if [[ ${Check} -eq 0 ]]; then
            FindFile $i
        fi
    else 
        exit 1
    fi
done

printf "%d:%s:%s\n" ${Max[0]} ${MaxString[0]} ${Fileinfo[0]}
printf "%d:%s:%s\n" ${Max[1]} ${MaxString[1]} ${Fileinfo[1]}
printf "%d:%s:%s\n" ${Max[2]} ${MaxString[2]} ${Fileinfo[2]}

