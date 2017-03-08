#!/bin/bash

export LD_LIBRARY_PATH=/home/itcast/lsystem/day05/session_daemon_test

while true; do
    #server='ps aux | grep daemon_d | grep -v grep'
    server=`ps aux | grep daemon_d | grep -v grep`
    if [ ! "$server" ]; then
        #nohup ./daemon_d -c 1 &
        nohup ./daemon_d &
        sleep 1
    fi
    sleep 2
done

