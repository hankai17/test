#! /bin/bash
echo $*
echo $@

for N in "$*"
do
   echo $N
done

for N in "$@"
do
   echo $N
done

echo $*
$*
echo $?
