package main

import (
	"fmt"
	"net/http"
	"log"
	"reflect"
	"bytes"
)

func main() {

	resp, err := http.Get("http://www.baidu.com")
	if err != nil {
		// handle error
		log.Println(err)
		return
	}

	defer resp.Body.Close()

	headers := resp.Header

	for k, v := range headers { //遍历响应头
		fmt.Printf("%v==%v\n", k, v)
	}

	fmt.Printf("resp proto %s\n", resp.Proto)
	fmt.Printf("resp status %s,status_code %d\n", resp.Status, resp.StatusCode)
	fmt.Printf("resp content length %d\n", resp.ContentLength)
	fmt.Printf("resp transfer encoding %v\n", resp.TransferEncoding)
	fmt.Printf("resp Uncompressed %t\n", resp.Uncompressed)
	fmt.Println(reflect.TypeOf(resp.Body)) //看body类型 *http.gzipReader 

	buf := bytes.NewBuffer(make([]byte, 0, 512))
	length, _ := buf.ReadFrom(resp.Body)

	fmt.Println(len(buf.Bytes()))
	fmt.Println(length)
	//fmt.Println(string(buf.Bytes()))
}

/*
请求
Host: www.baidu.com
User-Agent: Go-http-client/1.1
Accept-Encoding: gzip

响应
Date: Tue, 30 Jan 2018 10:27:46 GMT
Content-Type: text/html; charset=utf-8
Transfer-Encoding: chunked
Connection: Keep-Alive
Vary: Accept-Encoding
Set-Cookie: BAIDUID=DF4673498FAA944D85BAFA0C7EE2B6CF:FG=1; expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/; domain=.baidu.com
Set-Cookie: BIDUPSID=DF4673498FAA944D85BAFA0C7EE2B6CF; expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/; domain=.baidu.com
Set-Cookie: PSTM=1517308066; expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/; domain=.baidu.com
Set-Cookie: BDSVRTM=0; path=/
Set-Cookie: BD_HOME=0; path=/
Set-Cookie: H_PS_PSSID=1434_13549_21110_17001; path=/; domain=.baidu.com
P3P: CP=" OTI DSP COR IVA OUR IND COM "
Cache-Control: private
Cxy_all: baidu+d2831460d6d60fb7194ed1541960b2dc
Expires: Tue, 30 Jan 2018 10:27:24 GMT
X-Powered-By: HPHP
Server: BWS/1.1
X-UA-Compatible: IE=Edge,chrome=1
BDPAGETYPE: 1
BDQID: 0xd7a7150300036119
BDUSERID: 0
Content-Encoding: gzip
*/
