#!/bin/bash

kill -SIGKILL $(cat $HOME/bin/alarmservices.pid)
