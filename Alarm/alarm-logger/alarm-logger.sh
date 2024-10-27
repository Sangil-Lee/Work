#!/bin/sh
#
# Alarm Logger launcher for Linux or Mac OS X

# When deploying, change "TOP"
# to the absolute installation path
#TOP="."
TOP="$HOME/phoebus/services/alarm-logger"

# Ideally, assert that Java is found
# export JAVA_HOME=/opt/jdk-9
# export PATH="$JAVA_HOME/bin:$PATH"

if [ -d $TOP/target ]
then
  TOP="$TOP/target"
fi

JAR=`echo "${TOP}/service-alarm-logger-*.jar"`

java -jar $JAR "$@"
