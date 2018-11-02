#include "mtasker.hh"
#include <stdio.h>
#include <iostream>

template<class EventKey, class EventVal>
int MTasker<EventKey,EventVal>::waitEvent(const EventKey &key, EventVal *val, unsigned int timeout) //timeout默认为0 默认val为0
{
  Waiter w; //waiter里有一个上下文和tid 
  w.context=new ucontext_t;
  w.ttd= timeout ? time(0)+timeout : 0;  //立即死亡
  w.tid=d_tid;
  
  d_waiters[key]=w; //把waiter线程 存放到与之对应的事件id 的map容器中
  
  swapcontext(d_waiters[key].context,&d_kernel); // 'A' will return here when 'key' has arrived, hands over control to kernel first
                                                //因为是wait所以 切换到内核上下文
                                                //内核上下文继续运行 把tidpop出运行队列
                                                //保存当前上下文到后继上下文waiter中
  if(val && d_waitstatus==Answer)  //判断 等待状态  
    *val=d_waitval;                      //看过send之后还是有些模糊
  d_tid=w.tid;
  return d_waitstatus;
}

template<class Key, class Val>
void MTasker<Key,Val>::yield() {
  d_runQueue.push(d_tid); //把当前tid"线程"push到运行队列尾部
  swapcontext(d_threads[d_tid],&d_kernel); // give control to the kernel
                                            //切换到内核上下文(而此时的内核上下文是240行 后schedule函数返回 再次在schedule中大循环) 保存当前上下文到后继上下文(注意：此时的后继上下文是：167行那个"线程"的上下文 . 正是这个地方实现了线程切换)
}

template<class EventKey, class EventVal>
int MTasker<EventKey,EventVal>::sendEvent(const EventKey& key, const EventVal* val/*default 0*/) {
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

template<class Key, class Val>
void MTasker<Key,Val>::makeThread(tfunc_t *start, void* val) {
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


//上面的上下文 和 回调设置好后 再调用调度器
template<class Key, class Val>
bool MTasker<Key,Val>::schedule() {
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

template<class Key, class Val>
bool MTasker<Key,Val>::noProcesses() {
  return d_threads.empty();
}

template<class Key, class Val>
unsigned int MTasker<Key,Val>::numProcesses() {
  return d_threads.size();
}


template<class Key, class Val>
void MTasker<Key,Val>::getEvents(std::vector<Key>& events) {
  events.clear();
  for(typename waiters_t::const_iterator i=d_waiters.begin();i!=d_waiters.end();++i) {
    events.push_back(i->first);
  }
}


template<class Key, class Val>
void MTasker<Key,Val>::threadWrapper(MTasker *self, tfunc_t *tf, int tid, void* val) {
  (*tf)(val);
  self->d_zombiesQueue.push(tid); //tf回调函数返回时候才会到这一步 还是有个超时
                                //超时到了 才到这一步
  std::cout<<"push into zombies"<<std::endl;
  // we now jump to &kernel, automatically
}

template<class Key, class Val>
int MTasker<Key,Val>::getTid() {
  return d_tid;
}
