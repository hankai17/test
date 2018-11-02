#!/bin/bash
user=`whoami`
pid=`ps -u $user | grep demo | awk {'print $1'}`
if [ -z $pid ]
then
    echo "cgi not start"
    exit 1
fi
echo $pid
kill -9 $pid
echo "cgi over"
