#!/bin/bash

#file="/etc/passwd"
##delimiter ':'
#while IFS=: read -r f1 f2 f3 f4 f5 f6 f7
#do
#	printf 'UserName: %s, Shell: %s, HomeDir: %s\n' "$f1" "$f7" "$f6"
#done < "$file"

OPING_HOSTFILE="hostlist.txt"
OPING_OUTFILE="reply.txt"

rm -f "$OPING_OUTFILE"

OPing()
{
	oping -c 1 -f "$OPING_HOSTFILE" -O "$OPING_OUTFILE" >> "/dev/null"
}

ReadFile()
{
	#opingFile=$1
	opingFile=$OPING_OUTFILE

	while IFS=, read -r f1 f2 f3
	do
		#printf 'What: %s, IP: %s, Result: %s\n' "$f1" "$f2" "$f3"
		result=`echo "$f3 < 0" | bc`
		if [ "$result" == "0" ];
		then
			#printf 'Connected IP: %s\n' "$f2"
			echo "Conntected IP: $f2" | tr -d '"'
		fi
	done < "$opingFile"
}

OPing
ReadFile
