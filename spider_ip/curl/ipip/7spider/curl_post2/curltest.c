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

int main()
{
    // init CURL object
    CURL* curl = curl_easy_init();

    // set URL
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:10098/aaa/bbb.html");

    // perform: submit request to server
    // connect(server)
    // send(...);
    // recv(...);
    // printf(...)
    CURLcode code = curl_easy_perform(curl);
}
