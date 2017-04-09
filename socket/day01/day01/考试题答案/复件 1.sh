#!/bin/bash
#创建目录
i=0
while [ $i -lt 50 ]
do
	i=`expr $i + 1`
	mkdir -p ./目录/user$i
	chmod 754 ./目录/user$i
done





#!/bin/sh
i=1
groupadd class1
while [ $i -le 2 ]
do
        if [ $i -le 9 ]; then
                USERNAME=stu0${i}
        fi
        useradd $USERNAME
        mkdir /home/$USERNAME
        chown -R $USERNAME /home/$USERNAME
        chgrp -R class1 /home/$USERNAME
        i=$(($i + 1))
done


1、内核pcb进程块中有 block和pending状态字、忽略标志

2、当我们程序员手工设置blocking状态字的时候（sigprocmask(SIG_BLOCK,&bset,NULL);）
	用户空间的信号集sigset_t bset;会和内核空间的blocking状态字进行 或操作
3、当有2好信号给到应用程序的时候，因信号阻塞，所以pending状态字。。。对应位置1
	表示有未达信号

4、linux内核对未达信号的处理
	看你的信号是可靠信号，还是非可靠信号
	可靠信号 8k 引申linux内核默认每一个进程只能支持打开1024个文件描述符；
		socket的性能瓶颈，不言而喻。。。。。open files                      (-n) 1024
		（epoll）
	当信号是非可靠信号，发三条留下一条



信号默认动作 通过man 7 signal 进行观察研读
    Signal     Value     Action   Comment

	