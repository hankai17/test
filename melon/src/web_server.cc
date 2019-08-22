#include "web_server.hh"
#include <vector>
#include <stdio.h>

struct connection_thread_data {
  WebServer* web_server;
  Socket* client;
};

WebServer::WebServer(const string& local_addr, int port) : d_server(NULL) {
  d_listenaddress = local_addr;
  d_port = port;
}

void WebServer::bind() {
  try {
	d_server = create_server();
	cout<<"Listening for HTTP requests on "<<d_server->d_local.to_string_with_port()<<endl;
  }
  catch(NetWorkError &e) {
	cout<<"Listening on HTTP socket failed: "<<e.what()<<endl;
	d_server = NULL;
  }
}

void WebServer::go() {
  if(!d_server) return;
  try {
	//pthread_t tid;
	//acl TODO

	while(true) {
	  connection_thread_data* data = new connection_thread_data;
	  data->web_server = this;
	  data->client = d_server->accept();
	  //pthread_create(&tid, 0, &WebServerConnectionThreadStart, (void *)data); //我去！！！ 每条连接建一个线程 这不是坑我吗？？？？
	}
  }
  catch(NetWorkError &e) {
	cout<<"webserver thread: "<<e.what()<<endl;
  }
  catch(std::exception &e) {
	cout<<"STL Exception in webserver thread: "<<e.what()<<endl;
  }
  catch(...) {
	cout<<"Unknown exception in main webserver thread"<<endl;
  }
  exit(1);
}

void WebServer::server_connect(Socket* client) //主动套接字
	try {
	  client->set_non_blocking();
	  int bytes;
	  int timeout = 5;
	  char buf[1024];
	  bytes = client->read_with_timeout(buf, sizeof(buf), timeout);
	  if (bytes > 0) {
		string data = string(buf, bytes);
	  } else {
		// read error OR EOF
		//break;
	  }

	  ostringstream ss("ok");
	  string reply = ss.str();
	  client->writen_with_timeout(reply.c_str(), reply.size(), timeout);
	}
catch(NetWorkError &e) {
  cout<<"HTTP Exception: "<<e.what()<<endl;
}
catch(std::exception &e) {
  if(strstr(e.what(), "timeout")==0)
  cout<<"HTTP STL Exception: "<<e.what()<<endl;
}
catch(...) {
  cout<<"HTTP: Unknown exception"<<endl;
}

