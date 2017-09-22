#include <ucontext.h>  
#include <stdio.h>  
  
void func1(void * arg)  
{  
    puts("1");  
    puts("11");  
    puts("111");  
    puts("1111");  
  
}  
void context_test()  
{  
    char stack[1024*128];  
    ucontext_t child,main;  
  
    getcontext(&child); //获取当前上下文  
    child.uc_stack.ss_sp = stack;//指定栈空间  
    child.uc_stack.ss_size = sizeof(stack);//指定栈空间大小  
    child.uc_stack.ss_flags = 0;  
    child.uc_link = &main;//设置后继上下文  
    //child.uc_link = NULL;//设置后继上下文  
  
    makecontext(&child,(void (*)(void))func1,0);//修改上下文指向func1函数  
  
    swapcontext(&main,&child);//切换到child上下文，保存当前上下文到main------ 这就是调度的核心：切换到child执行 执行完毕后跳到child所指向的后继上下文main 
                                //而main恰好设置在此 
    puts("main");//如果设置了后继上下文，func1函数指向完后会返回此处  
}  
/* 先执行上下文中的fun回调 再执行后继上下文
*/
  
int main()  
{  
    context_test();  
  
    return 0;  
}  
