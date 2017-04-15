#include <signal.h>
#include "sstuff.hh"
#include <vector>
#include "mtasker.hh"
#include "misc.hh"
#include <algorithm>


//! Key to use in the demo webserver to distinguish events
struct WSEvent
{
  enum {Read,Write} what;
  Socket *who;
};

/* We need to provide this operator because MTasker orders event keys */
bool operator<(const WSEvent &a, const WSEvent &b)
{
  if(a.what<b.what)
    return true;
  if(a.what>b.what)
    return false;

  if(a.who<b.who)
    return true;
  return false;
}

/* The MTaker! */
MTasker<WSEvent> MT;


//! A simple class that offers Buffered IO on top of the Socket class 
class BufferedIO
{
public:
  BufferedIO(Socket *sock)
  {
    d_buffer=new char[1024];
    d_socket=sock;
    d_pos=d_available=0;
  }
  ~BufferedIO()
  {
    delete[] d_buffer;
  }
  void writeLine(const string &line)
  {
    write(line.c_str(),line.size());
  }
  void write(const char *buf, unsigned int toWrite)
  {
    int written=0;
    int res=0;
    WSEvent ws;
    ws.who=d_socket;
    ws.what=WSEvent::Write;
    
    do {
      res=d_socket->tryWrite(buf+written,toWrite);  
      if(!res) {
	MT.waitEvent(ws);
	continue;
      }
      
      written+=res;
      toWrite-=res;
    }while(toWrite);
  }
  
  int getchar()
  {
    if(d_pos==d_available) {
      // we emptied the buffer, see that we get new stuff

      WSEvent ws;
      ws.who=d_socket;
      ws.what=WSEvent::Read;
      if(!MT.waitEvent(ws,0,5)) 
	throw NetworkError("Timeout!");
    
      int res=d_socket->read(d_buffer,1024);
      if(!res)
	return -1; 
      d_available=res;
      d_pos=0;
    }

    return d_buffer[d_pos++];    
  }

  void readLine(string &line)
  {
    line.clear();
    int c;
    for(;;) {
      c=getchar();
      if(c==-1)
	break;
      line.append(1,(char)c);
      if(c=='\n')
	break;
    }  
  }

private:
  char *d_buffer;
  Socket *d_socket;
  size_t d_pos, d_available;
};




void connection(void *p)
{
  Socket *client=(Socket *)p;
  int chunksize=65000;          /* disk IO chunk to do at any one time */

  vector<string> words;
  BufferedIO bio(client);

  try {
    string line;
    bio.readLine(line); // GET line
    stringtok(words,line," \t\r\n");
    if(words.size()<2) {
      cerr<<"Malformed GET line"<<endl;
      delete client;
      return;
    }
    string fname=words[1];
    if(fname=="/" || fname.find("../")!=string::npos) // brute force ../ cutout
      fname="index.html";

    cout<<"["<<MT.getTid()<<"] Request for file '"<<fname<<"'"; cout.flush();
    
    do {
      bio.readLine(line);
    }while(line!="\r\n");  // ignore other request lines

    FILE *fp=fopen(("./"+fname).c_str(),"r");
    if(!fp) {
      cout<<". Sending 404, not there"<<endl;
      bio.writeLine("HTTP/1.1 404 OK\r\n"
		    "Server: MTasker\r\n"
		    "Connection: close\r\n"
		    "Content-Type: text/html; charset=iso-8859-1\r\n\r\n");
    
      bio.writeLine("<H1>MTasker webserver</H1>\r\n");
      bio.writeLine("File '"+fname+"' is missing\r\n");
    }
    else {
      cout<<". Serving"; cout.flush();
      bio.writeLine("HTTP/1.1 200 OK\r\n"
		    "Server: MTasker\r\n"
		    "Connection: close\r\n"
		    "Content-Type: text/html; charset=iso-8859-1\r\n\r\n");

      vector<char>buffer(chunksize);
      int len;
      try {
	while((len=fread(&buffer[0],1,chunksize,fp))) {
	  bio.write(&buffer[0],len);
	}
      }
      catch(...) {
	fclose(fp);
	throw;
      }
      fclose(fp);
      cout<<". Done"<<endl;
    }
  }
  catch(NetworkError &ne) {
    cerr<<"Fatal error in connection thread: "<<ne<<endl;
  }
  delete client;
}

int main(int argc, char **argv)
{
  signal(SIGPIPE,SIG_IGN); //吊
  try {
    Socket s(InterNetwork, Stream); 
    IPEndpoint ep; //ip节点
    ep.address.byte=0;
    ep.port=8000;
    s.bind(ep);
    s.listen();
    s.setNonBlocking(); //socket非阻塞
    cerr<<"Bound to local port "<<ep.port<<endl;

    vector<WSEvent> waitevents;
    vector<Socket *>readers, writers,errors;
    Socket* newClient=0;

    for(;;) {
      while(MT.schedule());             // housekeeping, let threads run

      SelectSocketMultiplex mult; //文件描述符复用对象 
      mult.addReader(&s);               // make sure we listen on the main socket //监听被动套接字 是否有连接到来 ,这一步只是
					//挂到树上  当调用run时才真正select监听
      MT.getEvents(waitevents);         // get which events threads are waiting for

      /* add them to our SocketMultiplexor */
      for(vector<WSEvent>::const_iterator i=waitevents.begin();i!=waitevents.end();++i)  //遍历**事件 事件是读还是写 分别挂到
			  //相应的集下 
	if(i->what==WSEvent::Write) 
	  mult.addWriter(i->who);
	else if(i->what==WSEvent::Read) 
	  mult.addReader(i->who);

      /* and see if anything interesting happened */
      mult.run(readers,writers,errors); //开始selet 监听
      
      if(count(readers.begin(),readers.end(),&s)==1 && (newClient=s.accept())) { // activity on the main socket
	newClient->setNonBlocking();                                             // just in case
	MT.makeThread(connection,(void *)newClient);                             // spawn new thread for newClient
      }

      WSEvent ws;

      /* send Events for all read requests that were fullfilled */
      ws.what=WSEvent::Read;
      for(vector<Socket *>::iterator i=readers.begin();i<readers.end();++i) {
	if(*i==&s)
	  continue;
	ws.who=*i;
	MT.sendEvent(ws);
      }
      /* send Events for all write requests that were fullfilled */
      ws.what=WSEvent::Write;
      for(vector<Socket *>::iterator i=writers.begin();i<writers.end();++i) {
	ws.who=*i;
	MT.sendEvent(ws);
      }
    }
  }
  catch(NetworkError &ne) {
    cerr<<"Fatal error: "<<ne<<endl;
  }
  catch(exception &e) {
    cerr<<"STL Exception: "<<e.what()<<endl;
  }
}

