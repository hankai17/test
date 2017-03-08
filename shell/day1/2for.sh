#! /bin/sh

for FRUIT in apple banana pear; do
    echo "I like $FRUIT"
    #echo 'I like "$FRUIT"'
    #echo I like $FRUIT
done

:<< block

for FILENAME in `ls` ; do

    if [ -f "$FILENAME" ]; then
        echo "$FILENAME is a file"
    elif [ -d "$FILENAME" ]; then
        echo "$FILENAME is a dir"
    else
        echo "other file"
    fi

done

block

