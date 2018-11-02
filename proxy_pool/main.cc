#include<iostream>
#include<regex>
#include<map>
#include<string>
#include<curl/curl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<list>

#define user_agent "User-Agent: Mozilla/5.0 (Windows NT 6.1;WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"
pthread_mutex_t mutex;

struct proxy_node {
  std::string ip;
  int port;
  bool active;
  proxy_node(std::string p1, int p2): ip(p1) {
    port = p2;
    active = 1;
  }
};
//std::vector<proxy_node*> proxy_node_vec;
std::list<proxy_node*> proxy_node_vec;
std::map<std::string, int> node_map;

void print_node_map() {
  for (std::map<std::string,int>::iterator it = node_map.begin(); it != node_map.end(); it++) {
    std::cout<<it->first<<" "<<it->second<<std::endl;
  }
}
void insert_map(std::string ip,std::string port) {
  std::map<std::string, int>::iterator it;
  //node_map.clear();
  it = node_map.find(ip);
  if (it != node_map.end()) {
    //std::cout<<"warning insert complict ";
    //std::cout<<it->first<<"--"<<it->second<<std::endl;
  } else {
    std::pair<std::map<std::string, int>::iterator , bool > p = node_map.insert(std::pair<std::string,int>(ip,std::stoi(port)));
    if (p.second == true) {
      //std::cout<<"inserting success ip "<<ip<<" port "<<port<<" map.size() = "<<node_map.size()<<std::endl;
    }
  }
  return;
}

int write_data(char* page_data,size_t size, size_t nmemb,std::string* data) {
  if(data == NULL)
  return 0;
  data->append(page_data);
  return size * nmemb;
}

int write_data1(char* page_data,size_t size, size_t nmemb,std::string* data) {
  (void)page_data;
  (void)data;
  return size * nmemb;
}

int curl_fun(std::string ip, int port, std::string& buffer) {
  char str_port[6] = {0};
  sprintf(str_port,"%d",port);
  std::string proxy =  ip + ":" + std::string(str_port);
  long code;
  std::string http_test("www.baidu.com"); //proxy
  std::string spider_url("http://www.xicidaili.com/"); 
  std::string current_url = ip.size() == 0 ?  spider_url : http_test;
  //struct curl_slist *slist = NULL;
  //slist = curl_slist_append(slist, user_agent);
  CURL* curl = curl_easy_init();
  if (curl != NULL) {
    curl_easy_setopt(curl, CURLOPT_URL, current_url.data());
    if (ip.size() > 0) {
      curl_easy_setopt(curl, CURLOPT_PROXY, proxy.data());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data1);
    }
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 0);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer);  

    int unRet = curl_easy_perform(curl);
    if (CURLE_OK == unRet) {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
      if (code == 200) {
        //std::cout<<"success, will insert\n";
      } else {
        //std::cout<<"no success return code "<<code<<std::endl;
      }
    } else {
      //std::cout<<"curl perform error\n";
      return 0;
    }
    curl_easy_cleanup(curl);
  } else {
    //std::cout<<"curl init error\n";
    return 0;
  }
  return code;
}

void* callback(void* data) {
  proxy_node* p = (proxy_node*)data;
  std::string nul("");
  int ret = curl_fun(p->ip, p->port, nul); 
  if (ret == 200) {
    pthread_mutex_lock(&mutex);
    for (std::list<proxy_node*>::iterator it = proxy_node_vec.begin(); it != proxy_node_vec.end(); it++) {
      if ((*it)->ip == p->ip && (*it)->port == p->port) { //放到之前?
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
      }
    }
    proxy_node_vec.push_back(p);
    //std::cout<<"push_back into vec, vec.size = "<<proxy_node_vec.size() <<std::endl;
    pthread_mutex_unlock(&mutex);
  } else {
    delete p;
    p = NULL;
  }
  pthread_exit(NULL);
}

class big_container {
  public:
    std::map<std::string, int>::iterator it;
  public:
    big_container() {
    }

    void do_imm() {
    }

};
big_container* bcontainer = nullptr;

unsigned long str2int(std::string ip) {
  unsigned long ip_long = inet_network(ip.data());
  return ip_long;
}
void print_vec() {
  for (std::list<proxy_node*>::iterator it = proxy_node_vec.begin(); it != proxy_node_vec.end(); it++) {
    std::cout<<"in vec... "<<(*it)->ip<<"  "<<(*it)->port<<std::endl;
  }
}

void regex_c11(std::string& buffer) {
  if ((int)(buffer.size()) == 0) {
    return;
  }
  std::string ip("");
  std::string port("");
  std::regex re("<td>((\\d{1,3}\\.){3}\\d{1,3})</td>\\n.+<td>([0-9]+)");
  std::smatch result;
  while (regex_search(buffer, result, re)) {
    for (int i = 0; i < (int)result.size(); i++) {
      ip = i == 1 ? result[i] : ip;
      port = i == 3 ? result[i] : port;
    }
    insert_map(ip, port);
    buffer = result.suffix().str();
  }
  return;
}

void* spider_callback(void* data) {
  (void)data;
  while (1) {
    //detach 退出判断
    std::string buffer;
    int code = curl_fun("", 0, buffer);
    if (code == 200) {
      regex_c11(buffer);
    } else {
      std::cout<<"warning proxy web is not success\n";
    }

    pthread_t tid[node_map.size()];
    int i;
    std::map<std::string, int>::iterator it;
    //print_node_map();
    for (i = 0,it = node_map.begin(); it != node_map.end() ; i++,it++) {
      pthread_create(&tid[i], NULL, callback, new proxy_node(it->first,it->second));
    }

    for(i = 0; i < (int)node_map.size() ; i++) {
      pthread_join(tid[i], NULL);
    }

    print_vec();
    sleep(30);
  }
}

class spider_proxy {
  public:

    void do_spider() { //立即任务 //单线程 把爬到的ip port正则插入全局node_map 
      pthread_t tid;
      pthread_create(&tid, NULL, spider_callback, NULL); //生产者
      pthread_detach(tid);
      //pthread_create() //消费者 遍历vec 启线程探测 若不是200就摘掉 //写成并发的同spider_callback
      //join?
      //while(1); //在这把所有线程都跑了 要么就不是并发了
    }
};

void* check_callback(void* data) {
  std::string buffer("");
  //std::vector<proxy_node*>::iterator it = std::vector<proxy_node*>::iterator(*data);
  proxy_node* n = (proxy_node*)(data);
  int code = curl_fun(n->ip, n->port, buffer);
  pthread_exit((void*)code);
}

void* do_task(void* nul) {
  (void)nul;
  void* ret;
  while(1) {
    std::map<pthread_t, std::list<proxy_node*>::iterator> tid;
    std::vector<pthread_t> tidd;
    std::vector<std::list<proxy_node*>::iterator> will_delete;
    //std::vector<std:> will_delete;
    
    //tid.clear();
    //tidd.clear();
    for (std::list<proxy_node*>::iterator it = proxy_node_vec.begin(); it != proxy_node_vec.end(); it++) { //遍历也应该是原子操作?
      //std::cout<<"in vec... "<<(*it)->ip<<"  "<<(*it)->port<<std::endl;
      pthread_t id;
      pthread_create(&id, NULL, check_callback, (void*)(*it)); //不能传迭代器
      tid.insert(std::pair<pthread_t, std::list<proxy_node*>::iterator>(id, it));
      tidd.push_back(id);
    }

    for (int n = 0; n < (int)tid.size(); n++) {
      pthread_join(tidd[n], &ret);
      if ((int)ret != 200) {
        std::map<pthread_t, std::list<proxy_node*>::iterator>::iterator iter = tid.find(tidd[n]);
        std::cout<<(*(iter->second))->ip<<" "<<tidd[n]<<" 探测中不等于200"<<std::endl;
        //pthread_mutex_lock(&mutex);
        /*
        for (std::list<proxy_node*>::iterator it = proxy_node_vec.begin(); it != proxy_node_vec.end(); it++) {
          if ((*it)->ip == (*(iter->second))->ip) {
        pthread_mutex_lock(&mutex);
            proxy_node_vec.erase(it);
        pthread_mutex_unlock(&mutex);
            std::cout<<"将要删除!!!!\n";
            
          }
        }
        */
        std::map<pthread_t, std::list<proxy_node*>::iterator>::iterator it = tid.find(tidd[n]);
        pthread_mutex_lock(&mutex);
        will_delete.push_back(it->second);
        pthread_mutex_unlock(&mutex);

        /*
        std::map<pthread_t, std::vector<proxy_node*>::iterator>::iterator iter = tid.find(tidd[n]);
        if (iter != tid.end()) {
          proxy_node_vec.erase(iter->second);
        }
        pthread_mutex_unlock(&mutex);
        */
      }

    }
      //汇总删除
      std::cout<<"------will_delete size = "<<will_delete.size()<<std::endl;
      pthread_mutex_lock(&mutex);
      for (std::vector<std::list<proxy_node*>::iterator>::iterator it = will_delete.begin(); it != will_delete.end(); it++) {
          proxy_node_vec.erase(*it); //不能以迭代器形式删除 得判断 很有可能迭代器已经变了
      } 
      pthread_mutex_unlock(&mutex);
    print_vec();
    std::cout<<"~~~~~~~~~~pool size = "<<proxy_node_vec.size() <<std::endl;
    sleep(60);
  }
}

class check_proxy {
  public:

    void do_check() {
      pthread_t tid;
      pthread_create(&tid, NULL, do_task, NULL); //
      pthread_detach(tid);
    }
};

int main() 
{
  bcontainer = new big_container();
  pthread_mutex_init(&mutex, NULL);
  spider_proxy* spider = new spider_proxy();
  spider->do_spider();

  check_proxy* check = new check_proxy();
  check->do_check();

  while(1);
  pthread_mutex_destroy(&mutex);
  return 0;
}
