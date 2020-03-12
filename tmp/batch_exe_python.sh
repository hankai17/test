#!/bin/sh

command_error_exit() {
    $*
    if [ $? -ne 0 ]
    then
        exit 1
    fi
}

if [ $# -lt 2 ]
then
    echo "use [$0 date time]"
    echo "eg: sh 1.sh 20200312 0"
    exit 0
fi

date=$1
stime=$2

echo "will parser $date begin $stime"

if [ ! -d "/opt/logbak/ats/tmp" ]; then
    `mkdir /opt/logbak/ats/tmp`
fi

for file in `find /opt/logbak/ats/ -maxdepth 1 -type f -name "squid.*$date*" `
do
    start_time=`echo "$file" | cut -d . -f 4 | cut -d h -f 1`
    if [ ${start_time} -lt ${stime} ]; then
        continue
    fi
    ori_file=${file%.*}
    echo $ori_file
    `cp $file /opt/logbak/ats/tmp`
    `gzip -d /opt/logbak/ats/tmp/$file`

    `python /root/file/1.py /opt/logbak/ats/tmp/$ori_file > /opt/logbak/ats/tmp/$ori_file.txt`
done
