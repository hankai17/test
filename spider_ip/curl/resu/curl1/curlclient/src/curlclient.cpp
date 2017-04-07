/*
 * ================================================================================================
 *
 *    Description:  TODO
 *
 *        Version:  1.0.0
 *        Created:  2017-01-25
 *       Modified:  2017-02-08
 *       Revision:  none
 *       Compiler:  gcc-4.8.2
 *
 *         Author:  Zheng Yuhong , 371582812@qq.com
 *
 * ================================================================================================
 */

#include <map>

#include <curl/curl.h>
#include <curl/easy.h>

#include "curlclient.h"

//https://curl.haxx.se/libcurl/c/libcurl-tutorial.html

namespace curl {
namespace http {

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::string* sp = (std::string*)userdata;
    size_t ret = size * nmemb;
    sp->append(ptr, ret);
    return ret;
}

unsigned int CurlClient::_s_instance_num = 0;
std::mutex CurlClient::_s_mutex;

CurlClient::CurlClient() {
    //非线程安全，全局调用一次
    std::lock_guard<std::mutex> lock(_s_mutex);
    if (!_s_instance_num) {
        ++_s_instance_num;
        curl_global_init(CURL_GLOBAL_ALL);
    }
}

CurlClient::~CurlClient() {
    //非线程安全，全局调用一次
    std::lock_guard<std::mutex> lock(_s_mutex);
    if (_s_instance_num) {
        --_s_instance_num;
        curl_global_cleanup();
    }
}

int CurlClient::init(const std::string& host, const std::string& port,
        unsigned int timeout_ms, unsigned int max_retry) {
    _host = host;
    _port = port;
    _timeout_ms = timeout_ms;
    _max_retry = max_retry;
    return 0;
}

int CurlClient::http_request(const std::string& method,
        const std::string& url, 
        const std::map<std::string, std::string>& headers,
        const std::string& data,
        std::string& response) {

    CURL* curl = curl_easy_init();
    if (curl == NULL) {
        return CURL_EASY_INIT_ERR;
    }

    int ret = 0;
    do {
        std::string fullurl;
        if (_host.size() && _port.size()) {
            fullurl = "http://" + _host + ":" + _port;
            if (url.size() && url[0] != '/') {
                fullurl += "/";
            }
            fullurl += url;
        }
        else {
            fullurl = url;
        }
        curl_easy_setopt(curl, CURLOPT_URL, fullurl.c_str());

        //填充headers
        struct curl_slist *slist = NULL;
        std::map<std::string, std::string>::const_iterator it = headers.begin(); 
        for (; it != headers.end(); ++it){
            std::string hdr = it->first + ":" + it->second;
            slist = curl_slist_append(slist, hdr.c_str());
            if (slist == NULL) {
                ret = CURL_SLIST_APPEND_ERR;
                break;
            }
        }
        if (ret) {
            break;
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, _timeout_ms);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); //支持毫秒级别超时
        curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
        //默认情况下libcurl基于重用连接的考虑不会马上关闭

        //填充body
        if (method == "POST" || method == "PUT" || method == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        }

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        CURLcode code = curl_easy_perform(curl);
        for (unsigned int i = 0; i < _max_retry && code != CURLE_OK; ++i) {
            code = curl_easy_perform(curl);
        }
        if (code != CURLE_OK) {
            ret = CURL_EASY_PERFORM_ERR;
            break;
        }
    } while (false);

    curl_easy_cleanup(curl);
    return ret;
}

}//namespace http
}//namespace curl
