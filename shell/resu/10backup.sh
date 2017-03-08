#!/bin/bash
path=`pwd`
#echo "current path:${path}"
echo "current path:$path"

while [[ 1 -eq 1 ]];do
    #current files
    filelist=`ls 2>/dev/null`
    for pFile in $filelsit;do
        echo "current path:$path/$pFile"
        if [[ -d "$pFile" ]];then
            num=`ls -l | grep "^d" | wc -l`
            if [[ ${num} -gt 5 ]];then
                currentTime=`date +%Y%m%d%H%M%S`
                tarFileName="/home/itcast/backup/$pFile_$currentTime.tar.bz2"
                echo "backup files to $tarFileName"
                tar -czfv ${tarFileName} `find ${path}/${pFile} -mmin +60 -type d` 
                --remove files
            fi
        fi
    done
    sleep 600
done
