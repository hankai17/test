#!/bin/bash

echo "please input the file:"
read filename
#echo $filename
if [ -d $filename ];then
    echo "$filename is a directory"
elif [ -f $filename ];then
    echo "$filename is a file"
else
    echo "neither file nor directory"
fi
