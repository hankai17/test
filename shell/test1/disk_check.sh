#!/bin/bash
device_name=(`df -lh |grep  "^/dev/sd"|awk '{print $1}'`)
mount_dir=(`df -lh |grep  "^/dev/sd"|awk '{print $6}'`)
#echo ${device_name[@]}
#echo ${mount_dir[@]}
num=${#device_name[@]}
#echo $num

for ((i=0;i<=$num-1;i++));
do
touch ${mount_dir[$i]}/file1
        if [ $? -eq 0 ]
        then
        echo -e "\e[32m${device_name[$i]} is OK\e[0m"
        else
        echo -e "\e[31m${device_name[$i]} is error\e[0m"
        fi
rm -rf ${mount_dir[$i]}/file1
done
