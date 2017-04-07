/*==================================
*   Copyright (C) 2016 All rights reserved.
*
*   文件名称：curltest.c
*   创 建 者：薛国良
*   创建日期：2016年11月22日
*   描    述：
*
================================================================*/

#include <curl/curl.h>
#include <string.h>

// username:xxx, password:yyy

// FILE*
// fwrite fread
char buf[4096];
int idx = 0;

ssize_t callback(char* ptr, int m, int n, void* arg)
{
    memcpy(buf + idx, ptr, n*m);
    idx += n*m;
    buf[idx] = 0;
    return n*m;
}

/*
    {
        username: "xxx",
        password: "yyy"
    }
*/

int main()
{
    // init CURL object
    CURL* curl = curl_easy_init();

    // set URL
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:10098");

    // getdata to memory buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // use POST method
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    // POST data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "xxx");
   // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 7);

    // perform: submit request to server
    //
    // connect(server)
    // while(1)
    // {
    //  send(...);
    //  if(recv(...))
    //     break;
    //  callback(...)
    // }
    CURLcode code = curl_easy_perform(curl);
    printf("%s\n", buf);
}
