#include <pcre.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include <cstdio>
#include <fstream>
#include <errno.h>

class DomainRegex {
  public:
    DomainRegex()
      : _hits(0), _rex_string(NULL), _options(0), _rex(NULL), _extra(NULL), _next(NULL), _order(0) {}

    ~DomainRegex() {
      free(_rex_string);
      if(_rex) {
        pcre_free(_rex);
      }
    }

    void initialize(const std::string& reg) {
      _rex_string = strdup(reg.c_str());
    }

    //void increment() { ink_atomic_increment(&(_hits), 1); }
    void print(int ix, int max, const char *now) {
      fprintf(stderr, "[%s]:    Regex %d ( %s ): %.2f%%\n", now, ix, _rex_string, 100.0 * _hits / max);
    }

    int compile(const char **error, int *erroffset) {
      int ccount;

      //printf("rex_string: %s, optino: %d\n", _rex_string, _options);
      _rex = pcre_compile(_rex_string, // the pattern
                          _options,    // options
                          error,       // for error message
                          erroffset,   // for error offset
                          NULL);       // use default character tables
      if (NULL == _rex) { 
        printf("pcre_compile return -1\n");
        return -1; 
      }

      _extra = pcre_study(_rex, 0, error);
      if ((_extra == NULL) && (*error != 0)) { 
        printf("pcre_study return -1\n");
        return -1; 
      }
      if (pcre_fullinfo(_rex, _extra, PCRE_INFO_CAPTURECOUNT, &ccount) != 0) { 
        printf("pcre_fullinfo return -1\n");
        return -1; 
      }
      return 0;
    }

    int match(const char *str, int len, int ovector[]) {
      return pcre_exec(_rex,       // the compiled pattern
                       _extra,     // Extra data from study (maybe)
                       str,        // the subject string
                       len,        // the length of the subject
                       0,          // start at offset 0 in the subject
                       0,          // default options
                       ovector,    // output vector for substring information
                       30);        // number of elements in the output vector
    }

    void set_next(DomainRegex *next) { _next = next; }
    DomainRegex* next() const { return _next; }
    void set_order(int order) { _order = order; }
    int order() { return _order; }
    char* get_regex() { return _rex_string; }

  private:
    int _hits;
    char* _rex_string;
    int _options;

    pcre* _rex;
    pcre_extra* _extra;
    DomainRegex* _next;
    int _order;
};

DomainRegex* test() {
    std::ifstream f;
    std::string filename = "/opt/ats/etc/trafficserver/1.conf";
    size_t lineno = 0;
    const char* error;
    int erroffset;
    DomainRegex* cur;

    f.open(filename.c_str(), std::ios::in);
    if(!f.is_open()) {
      printf("open conf err %s\n", strerror(errno));
      return NULL;
    }
    while (!f.eof()) {
        std::string line;
        getline(f, line);
        ++lineno;
        if (line.size() == 0)
            continue;
    
        std::string::size_type pos1, pos2;
        pos1 = line.find_first_not_of(" \t\n");
        pos2 = line.find_first_of(" \t\n", pos1);
        std::string regex = line.substr(pos1, pos2 - pos1);
        printf("regex: %s\n", regex.c_str());
        if(regex.empty()) {
            continue;
        }
        cur = new DomainRegex();
        cur->initialize(regex);
        if(cur->compile(&error, &erroffset) < 0) {
            //printf("compile err\n");
            return NULL;
        }
     }
     return cur;
}

int main() {
  DomainRegex d;
  const char* err;
  int erroff;
  int ovector[30];
  std::string reg = "[a-z0-9A-Z]+\\.ifeng.com";
  const char* str = "sdw.ifeng.com";

  d.initialize(reg);
  d.compile(&err, &erroff);
  int ret = d.match(str, strlen(str), ovector);
  printf("regex: %s\nstr: %s\nmatch ret: %d\n", reg.c_str(), str, ret);
  printf("----------------\n");


  DomainRegex* cur = test();
  if(cur == NULL) {
    printf("can not get cur\n");
    return 0;
  }
  ret = cur->match(str, strlen(str), ovector);
  printf("regex: %s\nstr: %s\nmatch ret: %d\n", reg.c_str(), str, ret);
 

  return 0;
}

/*
[a-z0-9A-Z]+\.ifeng.com
*/
