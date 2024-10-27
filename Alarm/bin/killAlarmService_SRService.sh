tmux list-sessions | grep -v attached | awk 'BEGIN{FS=":"}{print $1}' | xargs -n 1 tmux kill-session -t || echo No sessions to kill
#tmux list-sessions | grep -v attached | awk 'BEGIN{FS=":"}{print $1}'
