#include "server_frame.hh"
#include "http_parser.hh"

void AsyncServerFrame::go() {
  if (!d_server) return;
  ((AsyncServer*)d_server)->async_wait_for_connections(d_fdm, boost::bind(&AsyncServerFrame::serve_connection1, this, _1));
}

void AsyncServerFrame::serve_connection(Socket* client) {
  client->set_non_blocking();
  string data;
  try {
	data.clear();
	int bytes = arecvtcp(data, 16384, client, true);
	if (bytes > 0) {
	  cout<<"bytes > 0"<<endl;
	} else {
	  // read error OR EOF
	  //break;
	}
  } catch (...) {
  }

  data = "resp";
  if (asendtcp(data, client) == -1 || data.empty()) {
	cout<<"Failed sending reply to HTTP client"<<endl;
  }
}

int url_deal_cb(http_parser* parser, const char *at, size_t length) {
  printf("at: %s\n", at);
}

int field_deal_cb(http_parser* parser, const char *at, size_t length) {
  printf("at: %s\n", at);
}


void AsyncServerFrame::serve_connection1(Socket* client) { //这个函数是起的协程
  cout<<"in cb serve_connection1"<<endl;
  client->set_non_blocking();
  string data;
  vector<string> words;
  int chunk_size = 65000;
  try {
	MIOBuffer mbuf(client); 
	mbuf.read_line(data);
    cout<<"data="<<data<<endl;
	stringtok(words, data," \t\r\n");
	if(words.size() < 2) {
      cout<<"words.size < 2"<<endl;
	  d_fdm->remove_read_fd(client->get_handle());
	  delete client;
      client = nullptr;
	  return;
	}
	string fname = words[1];
	if(fname == "/" || fname.find("../") != string::npos) fname = "index.html";

	do {
	  mbuf.read_line(data);
	} while(data != "\r\n");

    http_parser_settings settings;
    settings.on_url = url_deal_cb;
    settings.on_header_field = field_deal_cb;
    http_parser* parser = (http_parser*)malloc(sizeof(http_parser));
    http_parser_init(parser, HTTP_REQUEST);
    size_t nparsed = http_parser_execute(parser, &settings, data.c_str(), data.size());
    printf("--------->parser execute over\n");

	FILE* fp = fopen(("./" + fname).c_str(), "r");
	if(!fp) {
	  mbuf.write_line("HTTP/1.1 404 OK\r\n"
			"Server: MTasker\r\n"
			"Connection: close\r\n"
			"Content-Type: text/html; charset=iso-8859-1\r\n\r\n");
	  mbuf.write_line("<H1>MTasker webserver</H1>\r\n");
	  mbuf.write_line("File '" + fname + "' is missing\r\n");
	} else {
	  mbuf.write_line("HTTP/1.1 200 OK\r\n" //只有写入0个时候 才会发生waitEvent 不像是read_line 在读之前就开始waitEvent了  好奇怪
			"Server: MTasker\r\n"
			"Connection: close\r\n"
			"Content-Type: text/html; charset=iso-8859-1\r\n\r\n");
/*
	  vector<char> buffer(chunk_size);
	  int len;
	  try {
		while((len = fread(&buffer[0], 1, chunk_size, fp))) {
		  mbuf.write(&buffer[0], len);
		}
	  } catch(...) {
		fclose(fp);
		throw;
	  }
*/
	  fclose(fp);
	}
  } catch(NetWorkError &e) {
	cerr<<"Fatal error in serve connection :"<<e.what()<<endl;
  } catch(...) {
	cerr<<"Fatal error in serve connection : other"<<endl;
  }
  d_fdm->remove_read_fd(client->get_handle());
  delete client;
  client = nullptr;
}

