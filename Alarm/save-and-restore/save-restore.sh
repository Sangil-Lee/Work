#!/bin/sh
#
# Alarm Server launcher for Linux or Mac OS X

# When deploying, change "TOP"
# to the absolute installation path
TOP="$HOME/phoebus/services/save-and-restore"

# Ideally, assert that Java is found
# export JAVA_HOME=/opt/jdk-9
# export PATH="$JAVA_HOME/bin:$PATH"

if [ -d $TOP/target ]
then
  TOP="$TOP/target"
fi

JAR=`echo "${TOP}/service-save-and-restore-*.jar"`
#JAR=`echo "${TOP}/service-save-and-restore-4.7.3-SNAPSHOT.jar"`

java -jar $JAR "$@"
