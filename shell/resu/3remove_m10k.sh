#!/bin/bash
ls -l | awk '$5>1024 {print $9}'
#for Filename in $`ls -l | awk '$5>10240 {print $9}'`
#do
#    mv $Filename data_test
#done
#ls -la /data_test
echo "Done"
