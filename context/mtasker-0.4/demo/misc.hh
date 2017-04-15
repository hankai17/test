#ifndef MTASKER_MISC_HH
#define MTASKER_MISC_HH

#include <stdio.h>
#include <errno.h>
//#include <asm/page.h> 
#include <asm/poll.h> 
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>


//! Pure virtual interface for classes offering socket multiplexing services 
class SocketMultiplex
{
public:
  typedef vector<Socket *>svect_t;
  virtual ~SocketMultiplex(){}
  virtual void addReaders(svect_t& readers)=0;
  virtual void addWriters(svect_t& readers)=0;
  virtual void addErrors(svect_t& readers)=0;
  virtual void addReader(Socket *s)=0;
  virtual void addWriter(Socket *s)=0;
  virtual void addError(Socket *s)=0;
  virtual void run(svect_t& readers, svect_t& writers, svect_t& errors)=0;
};


//! Implementation of SocketMultiplex using select
/** Use this class to register interest in any number of Sockets and wait for availability on them */
class SelectSocketMultiplex : public SocketMultiplex
{
public:
  //! Add a group of Sockets that the caller wants to read from
  void addReaders(svect_t& readers)
  {
    adds(readers, d_readers); //前面的readers内容加到后面容器中
  }
  //! Add a group of Sockets that the caller wants to write to
  void addWriters(svect_t& readers)
  {
    adds(readers, d_writers);
  }
  //! Add a group of Sockets that the caller wants to receive error notifications of
  void addErrors(svect_t& readers)
  {
    adds(readers, d_errors);
  }

  //! Add a Socket the caller wants to read from
  void addReader(Socket *s)
  {
    add(s, d_readers);
  }
  //! Add a Socket the caller wants to write to
  void addWriter(Socket *s)
  {
    add(s, d_writers);
  }
  //! Add a Socket the caller wants to be notified about errors of
  void addError(Socket *s)
  {
    add(s, d_errors);
  }

  /** Wait for activity on the Socket classes registered 
      Wait for activity on the Socket classes registered 
      \param readers An svect_t that will be filled with Sockets ready for reading
      \param writers An svect_t that will be filled with Sockets ready for writing
      \param errors An svect_t that will be filled with Sockets which have errors 
  */
  void run(svect_t& readers, svect_t& writers, svect_t& errors) //添加这些集合 然后对其监听
  {
    fd_set rfds, wfds, efds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);
    
    int maxfd=0;
    for(svect_t::const_iterator i=d_readers.begin();i!=d_readers.end();++i) {
      maxfd=max((*i)->getHandle(),maxfd);
      FD_SET((*i)->getHandle(),&rfds); //往rfds集 里依次添加文件描述符
    }
    for(svect_t::const_iterator i=d_writers.begin();i!=d_writers.end();++i) {
      maxfd=max((*i)->getHandle(),maxfd);
      FD_SET((*i)->getHandle(),&wfds);
    }
    for(svect_t::const_iterator i=d_errors.begin();i!=d_errors.end();++i) {
      maxfd=max((*i)->getHandle(),maxfd);
      FD_SET((*i)->getHandle(),&efds);
    }

    struct timeval tv;
    tv.tv_usec=0;
    tv.tv_sec=1;
    int res=select(maxfd+1,&rfds,&wfds,&efds,&tv);
    if(res<0)
      throw NetworkError("Select: "+string(strerror(errno)));

    readers.clear(); //size变0 cap不变 内存不清空 
    writers.clear();
    errors.clear();

    if(res==0)
      return;
      
    for(svect_t::const_iterator i=d_readers.begin();i!=d_readers.end();++i) 
      if(FD_ISSET((*i)->getHandle(), &rfds)) //fd 是不是 rfds(传入传出)的一部分
	readers.push_back(*i);
    
    for(svect_t::const_iterator i=d_writers.begin();i!=d_writers.end();++i) 
      if(FD_ISSET((*i)->getHandle(), &wfds))
	writers.push_back(*i);

    for(svect_t::const_iterator i=d_errors.begin();i!=d_errors.end();++i) 
      if(FD_ISSET((*i)->getHandle(), &efds))
	errors.push_back(*i);

  }
private:

  void adds(svect_t &toadd, svect_t &addto) {
    for(svect_t::const_iterator i=toadd.begin();i!=toadd.end();++i) 
      add(*i, addto);
  }
  void add(Socket *s, svect_t& addto)
  {
    addto.push_back(s);
  }

  svect_t d_readers, d_writers, d_errors;

};


template <typename Container>
void
stringtok (Container &container, string const &in,
           const char * const delimiters = " \t\n")
{
  const string::size_type len = in.length();
  string::size_type i = 0;
  
  while (i<len) {
    // eat leading whitespace
    i = in.find_first_not_of (delimiters, i);
    if (i == string::npos)
      return;   // nothing left but white space
    
    // find the end of the token
    string::size_type j = in.find_first_of (delimiters, i);
    
    // push token
    if (j == string::npos) {
      container.push_back (in.substr(i));
      return;
    } else
      container.push_back (in.substr(i, j-i));
    
    // set up for next loop
    i = j + 1;
  }
}

#endif
