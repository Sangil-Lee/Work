#!/bin/bash

cd $HOME/bin
for index in {1..8}
do
	if test $index -eq 1
	then
		xterm -geometry 80x25+10+10 -e "start_ioc;bash" &
		echo $! > .tmp.pid
	elif test $index -eq 2
	then
		xterm -geometry 80x25+495+10 -e "start_alarm_accel;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 3
	then
		xterm -geometry 80x25+980+10 -e "start_alarm_demo;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 4
	then
		xterm -geometry 80x25+10+390 -e "start_alarm_logger;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 5
	then
		xterm -geometry 80x25+495+390 -e "start_acc_alarm_config;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 6
	then
		xterm -geometry 80x25+980+390 -e "start_demo_alarm_config;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 7
	then
		xterm -geometry 80x25+10+752 -e "start_save-restore;bash" &
		echo $! >> .tmp.pid
		tr '\n' ' ' < .tmp.pid > alarmservices.pid
#	else
#		echo "starting phoebus ... wait 20 seconds"
#		sleep 20
#		xterm -geometry 80x25+495+752 -e "phoebus;bash" &
	fi
done
