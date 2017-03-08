#!/bin/bash

file=`ls | grep -v cp_sim.sh|grep -v cp_adv.sh|grep -v file`
#echo $file
f=`ls | grep -v cp_sim.sh|grep -v cp_adv.sh|grep -v file| sed 's/.txt//'`
#echo $f

#: << block

for f in $f
do
    echo $f
    mkdir $f
    sub=".txt"
    txt="$f$sub"
    mv $txt $f
done
#compare | with sed 
#use more |

#block
