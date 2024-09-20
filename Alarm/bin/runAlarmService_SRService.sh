#!/bin/bash

runServices() {
	tmux new-session -d -s IOC -n win
	tmux split-window -h -t IOC:win
	tmux split-window -v -t IOC:win
	tmux send-keys -t IOC:win.0 "eps;cd $HOME/fishtank;./st.cmd" Enter
	tmux send-keys -t IOC:win.1 "eps;camonitor ctrlslab:tank" Enter
#	tmux attach-session -t IOC

	tmux new-session  -d -s alarm-server -n win
	tmux split-window -h -t alarm-server:win
	tmux send-keys -t alarm-server:win.0 "cd $HOME/alarm-server;./start_alarm_accel.sh" Enter
	tmux send-keys -t alarm-server:win.1 "cd $HOME/alarm-server;./start_alarm_demo.sh" Enter

	tmux new-session  -d -s alarm-logger -n win
	tmux split-window -h -t alarm-logger:win
	tmux send-keys -t alarm-logger:win.0 "cd $HOME/alarm-logger;./start_alarm_logger.sh" Enter

	tmux new-session -d -s SR -n win
	tmux split-window -h -t SR:win
	tmux send-keys -t SR:win.0 "cd $HOME/save-and-restore;./save-restore.sh" Enter

	tmux new-session -d -s alarm-conf-logger -n win
	tmux split-window -h -t alarm-conf-logger:win
	tmux send-keys -t alarm-conf-logger:win.0 "cd $HOME/alarm-config-logger;./acc_start_alarm_config.sh" Enter
	tmux send-keys -t alarm-conf-logger:win.1 "cd $HOME/alarm-config-logger;./demo_start_alarm_config.sh" Enter
}

runServices
