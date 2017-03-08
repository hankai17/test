#!/bin/bash

i=1
for i in `seq 4`
do
    dir="m$i"
    mkdir -p -v $dir
    sub=".txt"
    txt="$dir$sub"
    touch $txt
    mv $txt $dir
done
echo "success!"
