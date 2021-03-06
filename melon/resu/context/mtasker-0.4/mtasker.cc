#include "mtasker.hh"
#include <stdio.h>
#include <iostream>

/** \mainpage
    Simple system for implementing cooperative multitasking of functions, with 
    support for waiting on events which can return values.

    \section copyright Copyright and License
    MTasker is (c) 2004 by bert hubert. It is licensed to you under the terms of the GPL version 2.

    \section overview High level overview
    MTasker is designed to support very simple cooperative multitasking to facilitate writing 
    code that would ordinarily require a statemachine, for which the author does not consider 
    himself smart enough.

    This class does not perform any magic it only makes calls to makecontext() and swapcontext(). 
    Getting the details right however is complicated and MTasker does that for you.

    If preemptive multitasking or more advanced concepts such as semaphores, locks or mutexes
    are required, the use of POSIX threads is advised.

    MTasker is designed to offer the performance of statemachines while maintaining simple thread semantics. It is not
    a replacement for a full threading system.

    \section compatability Compatability
    MTasker is only guaranteed to work on Linux with glibc 2.2.5 and higher.
    It does not work on FreeBSD 4.x and notably, not on Red Hat 6.0. It may
    work on Solaris, please test. 

    \section concepts Concepts

    There are two important concepts, the 'kernel' and the 'thread'. Each thread starts out as a function,
    which is passed to MTasker::makeThread(), together with a possible argument.
The kernel is written by the developer, but can be quite short.

This function is now free to do whatever it wants, but realise that MTasker implements cooperative
multitasking, which means that the coder has the responsiblilty of not taking the CPU overly long.
    Other threads can only get the CPU if MTasker::yield() is called or if a thread sleeps to wait for an event, 
    using the MTasker::waitEvent() method.

\section kernel The Kernel
    The Kernel consists of functions that do housekeeping, but also of code that the client coder 
    can call to report events. A minimal kernel loop looks like this:
    \code
    for(;;) {
       MT.schedule();
       if(MT.noProcesses())  // exit if no processes are left
          break;
    }
    \endcode

    The kernel typically starts from the main() function of your program. New threads are also
    created from the kernel. This can also happen before entering the main loop. To start a thread,
    the method MTasker::makeThread is provided.

    \section events Events
    By default, Events are recognized by an int and their value is also an int.
    This can be overriden by specifying the EventKey and EventVal template parameters.
    
    An event can be a keypress, but also a UDP packet, or a bit of data from a TCP socket. The
    sample code provided works with keypresses, but that is just a not very useful example.

    A thread can also wait for an event only for a limited time, and receive a timeout of that 
    event did not occur within the specified timeframe.

    \section example A simple menu system
    \code
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
  
  for(;;) {
    while(MT.schedule()); // do everything we can do
    if(MT.noProcesses())  // exit if no processes are left
      break;

    if(!fgets(line,sizeof(line),stdin))
      break;
    
    MT.sendEvent(*line-'0');
  }
}
\endcode

\section example2 Canonical multitasking example
This implements the canonical multitasking example, alternately printing an 'A' and a 'B'. The Linux kernel
  started this way too.
\code
void printer(void *p)
{
  char c=(char)p;
  for(;;) {
    cout<<c<<endl;
    MT.yield();
  }

}

int main()
{
  MT.makeThread(printer,(void*)'a');
  MT.makeThread(printer,(void*)'b');

  for(;;) {
    while(MT.schedule()); // do everything we can do
    if(MT.noProcesses())  // exit if no processes are left
      break;
  }
}
\endcode

*/


//! puts a thread to sleep waiting until a specified event arrives
/** Threads can call waitEvent to register that they are waiting on an event with a certain key.
    If so desidered, the event can carry data which is returned in val in case that is non-zero.
    
    Furthermore, a timeout can be specified in seconds.
    
    Only one thread can be waiting on a key, results of trying to have more threads
    waiting on the same key are undefined.
    
    \param key Event key to wait for. Needs to match up to a key reported to sendEvent
    \param val If non-zero, the value of the event will be stored in *val
    \param timeout If non-zero, number of seconds to wait for an event.
    
    \return returns -1 in case of error, 0 in case of timeout, 1 in case of an answer 
*/
template<class EventKey, class EventVal>int MTasker<EventKey,EventVal>::waitEvent(const EventKey &key, EventVal *val, unsigned int timeout) //timeout默认为0 默认val为0
{
  Waiter w; //waiter里有一个上下文和tid 
  w.context=new ucontext_t;
  w.ttd= timeout ? time(0)+timeout : 0;  //立即死亡
  w.tid=d_tid;
  
  d_waiters[key]=w; //把waiter线程 存放到与之对应的事件id 的map容器中
  
  swapcontext(d_waiters[key].context,&d_kernel); // 'A' will return here when 'key' has arrived, hands over control to kernel first
                                                //因为是wait所以 切换到内核上下文
                                                //保存当前上下文到后继上下文waiter中
  if(val && d_waitstatus==Answer)  //判断 等待状态  
    *val=d_waitval;                      //看过send之后还是有些模糊
  d_tid=w.tid;
  return d_waitstatus;
}

//! yields control to the kernel or other threads
/** Hands over control to the kernel, allowing other processes to run, or events to arrive */

template<class Key, class Val>void MTasker<Key,Val>::yield()
{
  d_runQueue.push(d_tid); //把当前tid"线程"push到运行队列尾部
  swapcontext(d_threads[d_tid],&d_kernel); // give control to the kernel
                                            //切换到内核上下文(而此时的内核上下文是240行 后schedule函数返回 再次在schedule中大循环) 保存当前上下文到后继上下文(注意：此时的后继上下文是：167行那个"线程"的上下文 . 正是这个地方实现了线程切换)
}

//! reports that an event took place for which threads may be waiting
/** From the kernel loop, sendEvent can be called to report that something occured for which there may be waiters.
    \param key Key of the event for which threads may be waiting
    \param val If non-zero, pointer to the content of the event
    \return Returns -1 in case of error, 0 if there were no waiters, 1 if a thread was woken up.
*/
template<class EventKey, class EventVal>int MTasker<EventKey,EventVal>::sendEvent(const EventKey& key, const EventVal* val) //默认val 为0
{
  if(!d_waiters.count(key)) {
    return 0;
  }
  
  d_waitstatus=Answer;
  if(val)
    d_waitval=*val;
  
  ucontext_t *userspace=d_waiters[key].context; //因为发送的是key 根据key(tid)索引到相应的上下文 
  d_tid=d_waiters[key].tid;         // set tid 
  
  d_waiters.erase(key);             // removes the waitpoint 
  swapcontext(&d_kernel,userspace); // swaps back to the above point 'A'
  
  delete userspace;
  return 1;
}

//! launches a new thread
/** The kernel can call this to make a new thread, which starts at the function start and gets passed the val void pointer.
    \param start Pointer to the function which will form the start of the thread
    \param val A void pointer that can be used to pass data to the thread
*/
template<class Key, class Val>void MTasker<Key,Val>::makeThread(tfunc_t *start, void* val)
{
  ucontext_t *uc=new ucontext_t; //new一个上下文
  getcontext(uc); //获取当前上下文
  
  uc->uc_link = &d_kernel; // come back to kernel after dying
                            //设置后继上下文 为内核上下文(有种传入参数的味道)
  uc->uc_stack.ss_sp = new char[d_stacksize]; 
  
  uc->uc_stack.ss_size = d_stacksize;
#ifdef SOLARIS                                 //参数个数
  makecontext (uc, (void (*)(...))threadWrapper, 4, this, start, d_maxtid, val); //修改上下文指向threadWrapper函数
#else
  makecontext (uc, (void (*)(void))threadWrapper, 4, this, start, d_maxtid, val);
#endif
  d_threads[d_maxtid]=uc; //保存此上下文到 线程map容器中
  d_runQueue.push(d_maxtid++); // will run at next schedule invocation
                                //把此上下文key值 push到运行队列中
}


//! needs to be called periodically so threads can run and housekeeping can be performed
/** The kernel should call this function every once in a while. It makes sense
    to call this function if you:
    - reported an event
    - called makeThread
    - want to have threads running waitEvent() to get a timeout if enough time passed 
    
    \return Returns if there is more work scheduled and recalling schedule now would be useful
      
*/
//上面的上下文 和 回调设置好后 再调用调度器
template<class Key, class Val>bool MTasker<Key,Val>::schedule()
{

  if(!d_runQueue.empty()) { //运行队列不空，取头"tid" 根据tid找到线程map容器相应的 "线程" 
    d_tid=d_runQueue.front(); //获得对头元素
    swapcontext(&d_kernel, d_threads[d_tid]); //切换到此上下文运行，保存当前上下文到内核上下文  即运行完后 切到这里
    d_runQueue.pop();  //对头运行完了 弹出对头
    return true;
  }
  if(!d_zombiesQueue.empty()) {
    delete[] (char *)d_threads[d_zombiesQueue.front()]->uc_stack.ss_sp;
    delete d_threads[d_zombiesQueue.front()];
    d_threads.erase(d_zombiesQueue.front());
    d_zombiesQueue.pop();
    return true;
  }
  if(!d_waiters.empty()) {
    time_t now=time(0);
    for(typename waiters_t::const_iterator i=d_waiters.begin();i!=d_waiters.end();++i) {
      if(i->second.ttd && i->second.ttd<now) {
	std::cout<<"超时\n";
	d_waitstatus=TimeOut;
	swapcontext(&d_kernel,i->second.context); // swaps back to the above point 'A'
	delete i->second.context;              
	d_waiters.erase(i->first);                  // removes the waitpoint 
      }
	std::cout<<"没有超时\n";
    }
  }
std::cout<<"return false\n";
  return false;
}

//! returns true if there are no processes
/** Call this to check if no processes are running anymore
    \return true if no processes are left
 */
template<class Key, class Val>bool MTasker<Key,Val>::noProcesses()
{
  return d_threads.empty();
}

//! returns the number of processes running
/** Call this to perhaps limit activities if too many threads are running
    \return number of processes running
 */
template<class Key, class Val>unsigned int MTasker<Key,Val>::numProcesses()
{
  return d_threads.size();
}


//! gives access to the list of Events threads are waiting for
/** The kernel can call this to get a list of Events threads are waiting for. This is very useful
    to setup 'select' or 'poll' or 'aio' events needed to satisfy these requests.
    getEvents clears the events parameter before filling it.

    \param events Vector which is to be filled with keys threads are waiting for
*/
template<class Key, class Val>void MTasker<Key,Val>::getEvents(std::vector<Key>& events)
{
  events.clear();
  for(typename waiters_t::const_iterator i=d_waiters.begin();i!=d_waiters.end();++i) {
    events.push_back(i->first);
  }
}


template<class Key, class Val>void MTasker<Key,Val>::threadWrapper(MTasker *self, tfunc_t *tf, int tid, void* val)
{
  (*tf)(val);
  self->d_zombiesQueue.push(tid); //tf回调函数返回时候才会到这一步 还是有个超时
                                //超时到了 才到这一步
  std::cout<<"push into zombies"<<std::endl;
  
  // we now jump to &kernel, automatically
}

//! Returns the current Thread ID (tid)
/** Processes can call this to get a numerical representation of their current thread ID.
    This can be useful for logging purposes.
*/
template<class Key, class Val>int MTasker<Key,Val>::getTid()
{
  return d_tid;
}
