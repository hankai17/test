#! /bin/sh

foo() {
    echo "------start"

    echo "---$0"
    echo "---$1"
    echo "---$2"
    echo "---$3"
    echo "---$4"

    echo "------end"
}

foo aa bb cc dd

:<< block

echo "---$0"
echo "---$1"
echo "---$2"
echo "---$3"
echo "---$4"

block

