#!/bin/bash

sudo systemctl start zookeeper.service

sleep 3
sudo systemctl start kafka.service

sleep 3
tmux new-session -d -s Elastic -n win
tmux split-window -h -t Elastic:win
tmux send-keys -t Elastic:win.1 "cd /home/ctrluser/elasticsearch/bin;./elasticsearch" Enter
