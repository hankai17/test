#!/bin/bash
sudo groupadd class1
i=1
while [ $i -le 20 ];do
    if [ $i -le 10 ];then
        username="std0"$i
    else
        username="std"$i
    fi
    sudo useradd -g class1 -m $username 
    i=$((i+1))
done

