#!/bin/sh

set -ex

SECONDS=`date -u  '+%s'`
(
echo "// MACHINE GENERATED -- DO NOT EDIT"
echo "#define SOFTWARE_BUILD_DATE $SECONDS"
) >softwareBuildDate.h
