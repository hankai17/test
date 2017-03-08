#!/bin/bash

#mkdir userdata
cd userdata
i=1
while [ $i -le 50 ]
do
    #mkdir user$i
    rm -rf user$i
    #chmod 754 user$i
    i=$((i+1))
done
