#! /bin/bash

echo "Enter yes or no"

read YES_OR_NO

case "$YES_OR_NO" in 

    Yes|YES|y|Y|yes)
        echo "It's yes" ;;

    [nN]?)
        echo "it is no";;

    *)
        echo "other ..."
esac

