#!/bin/bash

runQtProcess() {
      echo "run Qt process ... "
      tmux new-session -d -s IOC -n win
      tmux split-window -h -t IOC:win
      tmux split-window -v -t IOC:win
      tmux send-keys -t IOC:win.0 "eps;cd $HOME/epicsQt/QtProcessScreen/iocBoot/testioc;./st.cmd" Enter
      sleep 5
      tmux send-keys -t IOC:win.1 "cd $HOME/epicsQt/QtProcessScreen/bin/linux-x86_64;./QtProcessScreen" Enter
}

runQtProcess
