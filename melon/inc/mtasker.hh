#pragma once
#include <signal.h>
#include <ucontext.h>
#include <queue>
#include <vector> 
#include <map>
#include <time.h>

template<class EventKey=int, class EventVal=int> class MTasker {
  private:
    ucontext_t d_kernel; //上下文 
    std::queue<int> d_runQueue; //运行队列
    std::queue<int> d_zombiesQueue; //僵尸队列

    struct Waiter {
      ucontext_t *context;
      time_t ttd; //time to die
      int tid;
    };

    typedef std::map<EventKey, Waiter> waiters_t; //服务员是map容器 key为传入参数 val为上下文指针
    waiters_t d_waiters; //key:事件id val:waiter线程
    std::map<int,ucontext_t*> d_threads; //"线程(上下文)"map容器 即"线程" //与服务员好像 // key:线程id val:线程上下文
    int d_tid;
    int d_maxtid;
    size_t d_stacksize;

    EventVal d_waitval; //val是传入的第二个参数
    enum {Error=-1,TimeOut=0,Answer} d_waitstatus;

  public:
    MTasker(size_t stacksize = 8192) : d_stacksize(stacksize) {
      d_maxtid=0;
    }

	using func_t = void (void*); //typedef void tfunc_t(void *);
    int waitEvent(const EventKey &key, EventVal *val=0, unsigned int timeout=0);
    void yield();
    int sendEvent(const EventKey& key, const EventVal* val=0);
    void getEvents(std::vector<EventKey>& events);
    void makeThread(func_t *start, void* val);
    bool schedule();
    bool noProcesses();
    unsigned int numProcesses();
    int getTid();
  private:
    static void threadWrapper(MTasker *self, func_t *tf, int tid, void* val);
};
#include"mtasker.cc"
