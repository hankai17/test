#!/bin/bash

echo "hankai"
echo "myfirstshell.sh"
echo "learn shell"
#echo "hollo root user:$user"
echo "hollo user:`whoami`"
echo $(date)
echo "time:`date`"
echo "cal:`cal`"
echo "pc_name:`uname -n`"
echo "os_name_ed:`uname -s;uname -r`"
echo "file:`ls ../`"
echo "process:`ps aux| grep root`"
echo "TERM:$TERM"
echo "PATH:$PATH"
echo "HOME:$HOME"
echo "disk:`df`"
echo "group id:`id -g`"
echo "bye!"
