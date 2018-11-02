#!/bin/bash
user=`whoami`
pid=`ps -u $user | grep demo | awk {'print $1'}`
if [ -z $pid ]
then
    spawn-fcgi -a 127.0.0.1 -p 8011 -f ./demo
    netstat -ano | grep 8011
else
    echo "demo has exist"
fi

