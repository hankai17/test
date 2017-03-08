#!/bin/bash

echo "success"
dir="data_test"
filename=`date +%Y%m%d%H%M`
echo $filename 

curdir=`pwd`
echo $curdir

usr=`whoami`
echo $usr

uid=`id -u`
echo $uid

ip=`ifconfig eth0 | grep "inet"| cut -f 2 -d ":"|cut -f 1 -d " "`
echo $ip
#-f 2第二块 -d : 以：分割

ip1=`ifconfig eth0 | grep "inet"|grep -v "inet6"|cut -f 2 -d ":"|awk '{print $1}'`
echo $ip1

if [ ! -x "$dir" ];then
    mkdir $dir
echo mkdir success
fi
uptime | awk '{print $1}'
