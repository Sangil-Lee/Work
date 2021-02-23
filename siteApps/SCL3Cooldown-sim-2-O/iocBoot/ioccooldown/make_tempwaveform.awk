#!/bin/bash

rm -f $2
awk 'BEGIN \
{ 
}
{ printf "\"%s\",", $0} \
NR%3==0 {print ""}' \
$1 >> $2

