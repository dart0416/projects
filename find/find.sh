#!/bin/bash

UGABUGA="\033[1;36m" #light cyan
BLUE="\033[0;34m" #blue
RED="\033[0;31m" #red
NC="\033[1;37m" #white
STDOUT='1'
STDERR='2'

readonly PWD_ABS="/home/stefan/projects/find/find.sh" #full script path

path=$1
INPUT=$2

for filename in $path*
do

	if [[ -d $filename ]] ; then
		echo -e 2>>/dev/null "$UGABUGA $PWD $BLUE $filename: "
		cd $filename
		$PWD_ABS
		cd ..
	else
		echo -e 2>>/dev/null "$UGABUGA $PWD $RED $filename"
	fi
done;
