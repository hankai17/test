#! /bin/bash

echo Enter a passwd

read PSWD

num=2

while [ "$PSWD" != "af666" -a "$num" -ne 0 ]; do

    echo "it is not passwd , try again"

    num=$(($num -1))

    read PSWD


done


