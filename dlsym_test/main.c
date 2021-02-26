#include <stdio.h>
void print_message_wrap();
void first();
void second();

int main()
{
    first();
    second();
    print_message_wrap();
    return 0;
}

// gcc -c main.c

/*
1 全局符号介入
    linux下的动态链接器存在以下原则：当共享对象被load进来的时候，它的符号表会被合并到进程的全局符号表中(这里说的全局符号表并不是指里面的符号全部是全局符号，而是指这是一个汇总的符号表)，当一个符号需要加入全局符号表时，如果相同的符号名已经存在，则后面加入的符号被忽略。

2 dlsym函数
    查找符号的地址。对应于函数，即函数地址，对应于变量，即变量地址。通过传入RTLD_NEXT参数，在当前库之后load进来的动态库中寻找对应符号的地址，显然在这里找到的会是glibc中相关socket函数的地址(这句话在下文中的波折部分被证明是错的，原因是并不是只有glibc中有socket函数的定义，不过在这里这么理解没毛病)。

都是在运行时之后 且在main之前 
*/
