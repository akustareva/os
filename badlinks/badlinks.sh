#!/usr/bin/env bash
recurse() {
	if [ -z "$1" ]
	then 
		echo "You haven't specified directory"
		exit 0
	fi	
	for file in $1/*
	do
		if [ -d "$file" ]	
		then
			recurse $file
		elif [ -h "$file" -a $(stat -c %Y $file) -lt $(($(date +%s) - 604800)) ]
		then 
			echo $file
		fi
	done	
}
recurse $1