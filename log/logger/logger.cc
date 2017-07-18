#include "logger.hh"
#include "namespaces.hh"
#include "lock.hh"

pthread_once_t Logger::s_once;
pthread_key_t Logger::s_loggerKey; //线程键

Logger &theL(const string &pname)  
{
  static Logger l("", LOG_DAEMON);//para2 有默认参数notice 
  if(!pname.empty())
    l.setName(pname);
  return l;
}

void Logger::log(const string &msg, Urgency u)
{
  struct tm tm;
  time_t t;
  time(&t);
  tm=*localtime(&t);

  if(u<=consoleUrgency) {
    char buffer[50];
    strftime(buffer,sizeof(buffer),"%b %d %H:%M:%S ", &tm);
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    Lock l(&m); // the C++-2011 spec says we need this, and OSX actually does
    clog << string(buffer) + msg <<endl;
  }
  if( u <= d_loglevel && !d_disableSyslog ) {
    syslog(u,"%s",msg.c_str());
  }
}

void Logger::setLoglevel( Urgency u )
{
  d_loglevel = u;
}
  

void Logger::toConsole(Urgency u)
{
  consoleUrgency=u;
}

void Logger::open()
{
  if(opened)
    closelog();
  openlog(name.c_str(),flags,d_facility);
  opened=true;
}

void Logger::setName(const string &_name)
{
  name=_name;
  open();
}

void Logger::initKey()
{                       //键值指针        键值析构
  if(pthread_key_create(&s_loggerKey, perThreadDestructor))
  ;
}
/*
构造函数 参数：log_id log_facility
设置open_flag  log级别为None 初始化一个线程键
openlog(log_id, open_flag, log_facility)
*/
Logger::Logger(const string &n, int facility)
{
  opened=false;
  flags=LOG_PID|LOG_NDELAY;
  d_facility=facility;
  d_loglevel=Logger::None;
  d_disableSyslog=false;
  //consoleUrgency=Error;
  consoleUrgency=All;
  name=n;

  if(pthread_once(&s_once, initKey)) //一次性初始化: 初始线程键 一般线程键只初始化一次 若2次则出错
  ;
  open();
}

//L<<Logger::Alert<<id<<"---alert test "<<std::endl;
Logger& Logger::operator<<(Urgency u) //常见的打印
{
  getPerThread()->d_urgency=u;
  return *this;
}

void Logger::perThreadDestructor(void* buf)
{
  PerThread* pt = (PerThread*) buf;
  delete pt;
}
//一开始 一个新线程 buf为空
//new出一个 键值对象(PerThread) 绑定键值指针
//每个线程只有一个 L 表达不出来 就这个意思
Logger::PerThread* Logger::getPerThread()
{
  void *buf=pthread_getspecific(s_loggerKey);
  PerThread* ret;
  if(buf)
    ret = (PerThread*) buf;
  else {
    ret = new PerThread();
    pthread_setspecific(s_loggerKey, (void*)ret);
  }
  return ret;
}

Logger& Logger::operator<<(const string &s)
{
  PerThread* pt =getPerThread();
  pt->d_output.append(s);
  return *this;
}

Logger& Logger::operator<<(const char *s)
{
  *this<<string(s);
  return *this;
}

Logger& Logger::operator<<(int i)
{
  ostringstream tmp;
  tmp<<i;

  *this<<tmp.str();

  return *this;
}

Logger& Logger::operator<<(double i)
{
  ostringstream tmp;
  tmp<<i;
  *this<<tmp.str();
  return *this;
}

Logger& Logger::operator<<(unsigned int i)
{
  ostringstream tmp;
  tmp<<i;

  *this<<tmp.str();

  return *this;
}

Logger& Logger::operator<<(unsigned long i)
{
  ostringstream tmp;
  tmp<<i;

  *this<<tmp.str();

  return *this;
}

Logger& Logger::operator<<(unsigned long long i)
{
  ostringstream tmp;
  tmp<<i;

  *this<<tmp.str();

  return *this;
}

Logger& Logger::operator<<(long i)
{
  ostringstream tmp;
  tmp<<i;

  *this<<tmp.str();

  return *this;
}

Logger& Logger::operator<<(ostream & (&)(ostream &))
{
  PerThread* pt =getPerThread();

  log(pt->d_output, pt->d_urgency);
  pt->d_output.clear();
  pt->d_urgency=Info;
  return *this;
}

