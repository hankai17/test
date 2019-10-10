#!/bin/bash
#v1.0.1

if [ $# -lt 1 ] ; then
    echo "USAGE: $0 command line" 
    echo " e.g.: $0 python abc.py" 
    exit 1;
fi
#echo $#
#echo $$
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $DIR

while true; do
    sleep 1
    server=`ps -ef | grep "$*" | grep -v grep | grep -v "$$"`
    if [ ! "$server" ]; then
        #如果不存在就重新启动
        #nohup ./  .. >/dev/null 2>&1 &
        #echo "start"
        logger -p local3.info "runforever start $*"
        nohup $* >/dev/null 2>&1 &
        sleep 1
    fi
    #每次循环沉睡10s
    #sleep 1
done
