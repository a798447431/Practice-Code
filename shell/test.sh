#!/bin/bash
Max=0
MaxString=0

function Fliter() {
  FliterFile=$1
    file ${FliterFile} | grep -q -w "text"
  if [[ $? -eq 0 ]]; then
    echo 0
  else
    echo 1
  fi
}

function FindFile() {
  local File=$1
  for i in `cat ${File} | tr -c -s "A-Za-z" "\n"`; do
    echo $i
    if [[ ${Max} -lt ${#i} ]]; then
      Max=${#i}
      MaxString=${i}
    fi
  done
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

if [[  -d $1 ]]; then
  FindDir $1
elif [[ -f $1 ]]; then
  Check=`FliterFile $1`
  if [[ ${Check} -eq 0 ]]; then
    FindFile $1
  fi
fi

printf "MaxString=%s, Max=%d\n" ${MaxString} ${Max}
