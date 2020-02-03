#!/bin/bash 
echo "$*" | cut -d ' ' -f 2-
echo $#
echo $@
