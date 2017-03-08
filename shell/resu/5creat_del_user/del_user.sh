#!/bin/bash

i=1
while [ $i -le 20 ];do
    if [ $i -le 10 ];then
        username="std0"$i
    else
        username="std"$i
    fi
    sudo deluser $username
    sudo rm -rf /home/$username
    i=$((i+1))
done
sudo groupdel class1
    

