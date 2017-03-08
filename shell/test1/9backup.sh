#!/bin/bash
path=`pwd`
filelist=`ls -lh`
echo $filelsit
#echo "current path:${path}"
echo "current path:$path"

while [ 1 -eq 1 ]; do
    #filelist=`ls 2>/dev/null`
    echo "in while"
    for pFile in $filelsit;do
        echo "current path:$path/$pFile"
        if [[ -d "$pFile" ]];then
            tarFileName="/home/itcast/backup/${pFile}_{$currentTime}.tar.bz2"
            echo "backup files to $tarFileName"
            tar -czfv ${tarFileName} 
            mv ${tarFileName} /home/itcast/backup/
        fi
    done
    sleep 600
done
