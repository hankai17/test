#include "mtasker.hh"
#include <iostream>

using namespace std;

MTasker<> MT;

void menuHandler(void *p)
{
  int num=(int)p;
  cout<<"Key handler for key "<<num<<" launched"<<endl;
 
  MT.waitEvent(num);
  cout<<"Key "<<num<<" was pressed!"<<endl;
}


int main()
{
  char line[10];

  for(int i=0;i<10;++i) 
    MT.makeThread(menuHandler,(void *)i);

  int numWoken;
  for(;;) {
    while(MT.schedule()); // do everything we can do  
    std::cout<<"-=-=-=-=-=-=-=-\n"; //也就是说当 10个task都在wait时 就会从while中出来 //but why?
    if(MT.noProcesses())  // exit if no processes are left
      break;

    if(!fgets(line,9,stdin)) // read a keypress
      break;
    
    numWoken=MT.sendEvent(*line-'0'); // wake up waiters
    cout<<"Woke "<<numWoken<<" tasks"<<endl;
  }
  cout<<"No more tasks running"<<endl;
}
/*
Key handler for key 0 launched
Key handler for key 1 launched
Key handler for key 2 launched
Key handler for key 3 launched
Key handler for key 4 launched
Key handler for key 5 launched
Key handler for key 6 launched
Key handler for key 7 launched
Key handler for key 8 launched
Key handler for key 9 launched
-=-=-=-=-=-=-=-
1
Key 1 was pressed!
push into zombies
Woke 1 tasks
-=-=-=-=-=-=-=-
4
Key 4 was pressed!
push into zombies
Woke 1 tasks
-=-=-=-=-=-=-=-
*/

/*
makeThread  
    获取当前上下文 设置后继上下文kernel 设置指向函数 
    保存上下文(map<tid,上下文> 此map是dmap)和tid(保存到运行队列中) 
schedule
    遍历运行队列 
    根据对头tid找相应上下文  保存当前上下文到kernel    ------kernel在242行处
    运行上下文指向的函数(threadWrapper) 开始回调函数 
        但是回调函数里有waitEvent
        运行内核 保存当前上下文(map(key,上下文),此map是waitermap)  ------当前上下文(这个当前上下文在回调函数里)在map里
        因为运行内核 到242行处 tid弹出队列 return true 继续在while(schedule) 连续10次弹出变为空
        ......所以看到了连续输出10行 Key handler for key * launched

    因为d_waiter容器不是空了 所以有超时判断  又因为传入0超时 所以没有超时(也就是说永久阻塞等待)
    schedule返回false 

    sendEvent 根据send的key值获取上下文(d_waiters) 
    运行上下文(继续回调函数) 155行  此时状态为Answer 并 得到tid
    返回到305行 保存tid到僵尸队列(证明回调完成 一个线程/tid的完成)
    然后再次跳到内核  ......





*/
