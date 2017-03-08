#!/bin/bash

file_count=0
line_count=0

function fun_count()
{
    for file in `ls $1`
    do
        if [ -d $1"/"$file ];then
            fun_count $1"/"$file
        else
            declare -i filelines
        filelines=`sed -n '$=' $1"/"$file`
        line_count=$line_count+$filelines
        file_count=$file_count+1
    fi
done
}

if [ $# -gt 0 ];then
    for m_dir in $@
    do
        fun_count $m_dir
    done
else
    fun_count "."
fi
echo "file_count:$file_count"
echo "line_count:$line_count"


