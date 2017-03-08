#! /bin/bash

echo "0---$0"
echo "1---$1"
echo "2---$2"


set -x

echo "3---$3"
echo "4---$4"

echo "----$#"
echo "----$@"
echo "----$*"

set +x

echo "----$$"

cat

