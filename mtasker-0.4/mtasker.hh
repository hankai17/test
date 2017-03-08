#ifndef MTASKER_HH
#define MTASKER_HH
#include <signal.h>
#include <ucontext.h>
#include <queue>
#include <vector> 
#include <map>
#include <time.h>

//! The main MTasker class    
/** The main MTasker class. See the main page for more information.
    \param EventKey Type of the key with which events are to be identified. Defaults to int.
    \param EventVal Type of the content or value of an event. Defaults to int. Cannot be set to void.
    \note The EventKey needs to have an operator< defined because it is used as the key of an associative array
*/
template<class EventKey=int, class EventVal=int> class MTasker
{
private:
  ucontext_t d_kernel;     
  std::queue<int> d_runQueue;
  std::queue<int> d_zombiesQueue;

  struct Waiter
  {
    ucontext_t *context;
    time_t ttd;
    int tid;
  };

  typedef std::map<EventKey,Waiter> waiters_t;
  waiters_t d_waiters;
  std::map<int,ucontext_t*> d_threads;
  int d_tid;
  int d_maxtid;
  size_t d_stacksize;

  EventVal d_waitval;
  enum {Error=-1,TimeOut=0,Answer} d_waitstatus;

public:
  //! Constructor
  /** Constructor with a small default stacksize. If any of your threads exceeds this stack, your application will crash. 
      This limit applies solely to the stack, the heap is not limited in any way. If threads need to allocate a lot of data,
      the use of new/delete is suggested. 
   */
  MTasker(size_t stacksize=8192) : d_stacksize(stacksize)
  {
    d_maxtid=0;
  }

  typedef void tfunc_t(void *); //!< type of the pointer that starts a thread 
  int waitEvent(const EventKey &key, EventVal *val=0, unsigned int timeout=0);
  void yield();
  int sendEvent(const EventKey& key, const EventVal* val=0);
  void getEvents(std::vector<EventKey>& events);
  void makeThread(tfunc_t *start, void* val);
  bool schedule();
  bool noProcesses();
  unsigned int numProcesses();
  int getTid();
private:
  static void threadWrapper(MTasker *self, tfunc_t *tf, int tid, void* val);
};
#include "mtasker.cc"
#endif
