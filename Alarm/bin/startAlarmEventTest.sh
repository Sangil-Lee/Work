#!/bin/bash

cd $HOME/bin
for index in {1..3}
do
	if test $index -eq 1
	then
		xterm -geometry 80x25+10+10 -e "start_alarm_event_test;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 2
	then
		xterm -geometry 80x25+495+10 -e "start_alarm_event_test_logger;bash" &
		echo $! >> .tmp.pid
	elif test $index -eq 3
	then
		xterm -geometry 80x25+980+10 -e "start_alarm_event_test_config;bash" &
		echo $! >> .tmp.pid
		tr '\n' ' ' < .tmp.pid > alarmservices.pid
	fi
done
