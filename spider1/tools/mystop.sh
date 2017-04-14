#!/bin/bash

user=`whoami`
echo $user

pid=`ps -u $user | grep spider | awk {'print $1'}`
echo $pid

if [ -z $pid ]
then
        echo "spider have not start"
        exit 1
fi

kill -9 $pid
