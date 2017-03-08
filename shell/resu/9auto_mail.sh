#########################################################################
# File Name: 9auto_mail.sh
# Author: guaz
# mail: hankai17@126.com
# Created Time: 2017-02-15 17:49:12
#########################################################################
#!/bin/bash

DIR=/root
mailcontent=$DIR/mailcontent
> "$mailcontent"
echo -e "hello! \n" >> $mailcontent
echo -e "/t file: `date +%Y-%m-%d` , please certify." >> $mailcontent
cat mailcontent | /usr/bin/mutt -s "check report" -a /root/1.file hankai17@126.com -c
