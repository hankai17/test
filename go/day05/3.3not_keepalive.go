package main

import (
    "fmt"
    "net/http"
    "io/ioutil"
)

func main() {
    req, err := http.NewRequest("GET","http://golang.org", nil)
    if err != nil {
        fmt.Println(err)
        return
    }

    req.Close = true
    //or do this:
    //req.Header.Add("Connection", "close")

    resp, err := http.DefaultClient.Do(req)
    if resp != nil {
        defer resp.Body.Close()
    }

    if err != nil {
        fmt.Println(err)
        return
    }

    body, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        fmt.Println(err)
        return
    }

    fmt.Println(len(string(body)))
}

//http库默认对端关闭我才关闭 可通过设置请求变量中的Close域的值为true来让http库在请求完成时关闭连接  也就是说我默认发起的是一个长连接
//另一个选项是添加一个Connection的请求头并设置为close  也就是手动设置短连接

//也可以取消http的全局连接复用
func main1() {
    tr := &http.Transport{DisableKeepAlives: true}
    client := &http.Client{Transport: tr}

    resp, err := client.Get("http://golang.org")
    if resp != nil {
        defer resp.Body.Close()
    }

    if err != nil {
        fmt.Println(err)
        return
    }

    fmt.Println(resp.StatusCode)

    body, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        fmt.Println(err)
        return
    }

    fmt.Println(len(string(body)))
}
