#!/bin/bash
line1=`awk '{print NR}' many.sh | tail -n1`
echo "lines1:$line1"

#line2=`awk 'BEGIN{print NR}' many.sh`
line2=`awk 'END{print NR}' many.sh`
echo "lines2:$line2"

#line3=`grep -n "" many.sh|awk -F: '{print'}|tail -n1`
#echo "lines3:$line3"

line4=`sed -n '$=' many.sh`
echo "lines4:$line4"

line5=`wc -l many.sh`
echo "lines5:$line5"

line6=`cat many.sh|wc -l`
echo "lines6:$line6"


