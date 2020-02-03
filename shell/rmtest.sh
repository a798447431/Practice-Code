#!/bin/bash
Host=`hostname`
echo "${Host}"
Names=`cat /etc/passwd | awk -F: '{if ($3 > 1000 && $3 != 65534) printf("%s ", $1)}'`
for i in $Names; do
    File="/home/${i}/.ssh/authorized_keys"
    if [[ -f $File ]]; then
        rm -f ${File} && echo "Rm OK! $i"
    else 
        continue
    fi
done
