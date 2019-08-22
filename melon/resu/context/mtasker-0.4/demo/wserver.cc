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
class BufferedIO  //基于套接字的IO
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
		void writeLine(const string &line) //写到本对象的成员d_socket中
		{
			write(line.c_str(),line.size()); //非库函数  下面
		}
		void write(const char *buf, unsigned int toWrite)
		{
			int written=0;
			int res=0;
			WSEvent ws; //猜  肯定有读写 肯定有fd
			ws.who=d_socket;
			ws.what=WSEvent::Write;

			do {
				res=d_socket->tryWrite(buf+written,toWrite);  //重点函数tryWrite
				if(!res) { //写入0个 与 eagain 则都返回0 .说明有问题 则开始等待  //韩凯
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
	Socket *client=(Socket *)p;  //服务器端的主动套接字  有新连接到来了
	int chunksize=65000;          /* disk IO chunk to do at any one time */ //块大小

	vector<string> words;
	BufferedIO bio(client); //根据主动套接字初始化一个io对象 io肯定有读写方法
/*
line=GET / HTTP/1.1
words=GET
words=/
words=HTTP/1.1
[2] Request for file 'index.html'
line=Host: www.baidu.com
line=Connection: Close
line=
. Serving. Done
push into zombies
*/
	try { //解析客户端请求 非阻塞？  //这是重点 里面有事件 韩凯
		string line;
		bio.readLine(line); // GET line  //有wait  5s钟无反应则 放到僵尸队列 同时这5s钟运行内核 
		std::cout<<"line="<<line<<std::endl;
		stringtok(words,line," \t\r\n");
		if(words.size()<2) {
			//cerr<<"Malformed GET line"<<endl;
			cerr<<"err, words.size="<<words.size()<<endl;
			delete client;
			return;
		}for(auto& i : words){cout<<"words="<<i<<std::endl;}

		string fname=words[1];
		if(fname=="/" || fname.find("../")!=string::npos) // brute force ../ cutout
			fname="index.html";
		cout<<"["<<MT.getTid()<<"] Request for file '"<<fname<<"'"; cout.flush();

		do {
			bio.readLine(line);
			std::cout<<"line="<<line<<std::endl;
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

		vector<WSEvent> waitevents; //webserverevevt肯定有 读写 和 socket
		vector<Socket *>readers, writers,errors;
		Socket* newClient=0;

		for(;;) {
			while(MT.schedule());             // housekeeping, let threads run

			SelectSocketMultiplex mult; //文件描述符复用对象 
			mult.addReader(&s);               // make sure we listen on the main socket 
			//监听被动套接字 是否有连接到来 ,这一步只是
			//挂到树上  当调用run时才真正select监听
			//把s添加到mult内部元素 d_readers中 不用想 d_readers就是一个 socket的vec 容器
			MT.getEvents(waitevents);         // get which events threads are waiting for
			//喵喵喵？ //把MTasker中的d_waiters(map容器 key是事件id val是waiter结构) 遍历一遍 把事件id赋值给waitevents
			//也就是说这里的事件id 即是socket结构体了

			//参数是等待事件 容器 是传出参数  每次调用都会在内部先
			//清空此容器 
			//疑问： 一上来就获取事件  关键是 这些事件是在什么时机插入到MT内部的d_waiter map容器中的？？？
			/* add them to our SocketMultiplexor */
			for(vector<WSEvent>::const_iterator i=waitevents.begin();i!=waitevents.end();++i)  //遍历上面传出等待事件容器 事件是读还是写 分别挂到
				//相应的集下 
				if(i->what==WSEvent::Write) 
					mult.addWriter(i->who);
				else if(i->what==WSEvent::Read) 
					mult.addReader(i->who);

			/* and see if anything interesting happened */
			mult.run(readers,writers,errors); //开始selet 监听 
			//将结果socket结构体 push到mult的read/write/error
			//容器中
			//这里设计的跟select机制相同 也是传出参数
			//返回s出现的个数  
			//accept 返回第一个链接 
			if(count(readers.begin(),readers.end(),&s)==1 && (newClient=s.accept())) { // activity on the main socket
				newClient->setNonBlocking();                                             // just in case
				MT.makeThread(connection,(void *)newClient);                             // spawn new thread for newClient
				//拿到一个新链接做什么？ 先accept返回出主动套接字 再传参connect()函数 
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

