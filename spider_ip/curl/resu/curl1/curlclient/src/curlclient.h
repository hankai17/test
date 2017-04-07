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

#ifndef CURLCLIENT_CURLCLIENT_H
#define CURLCLIENT_CURLCLIENT_H

#include <string>
#include <map>
#include <mutex>

namespace curl {
namespace http {

const int CURL_EASY_INIT_ERR = -1;
const int CURL_SLIST_APPEND_ERR = -2;
const int CURL_EASY_PERFORM_ERR = -3;

class CurlClient {
private:
    static unsigned int _s_instance_num;
    static std::mutex _s_mutex;
    std::string _host;
    std::string _port;
    unsigned int _timeout_ms;
    unsigned int _max_retry;
public:
    CurlClient();
    ~CurlClient();
    int init(const std::string& host, const std::string& port,
            unsigned int timeout_ms, unsigned int max_retry);
    //host，port为空表示在http_request使用绝对的URL
    //host，port非空表示在http_request使用相对的URL
    //timeout_ms，连接超时参数，单位为毫秒
    //max_retry，最大重试次数
    
    int http_request(const std::string& method, //请求方法，支持GET，POST，PUT，DELETE
            const std::string& url, //请求URL，相对URL或者绝对URL
            const std::map<std::string, std::string>& headers, //请求HEAD
            const std::string& data, //请求body
            std::string& response);
};

}//namespace http
}//namespace curl

#endif//CURLCLIENT_CURLCLIENT_H
