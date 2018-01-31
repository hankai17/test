package main

import (
	"net/http"
	"strings"
	"fmt"
	"io/ioutil"
	"log"
	"encoding/json"
)


func main() {
	client := &http.Client{}

	req, err := http.NewRequest("POST", "http://www.maimaiche.com/loginRegister/login.do",
		strings.NewReader("mobile=xxxxxxxxx&isRemberPwd=1"))
	if err != nil {
		log.Println(err)
		return
	}

	req.Header.Set("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8") //post必加
	resp, err := client.Do(req)

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Println(err)
		return
	}

	fmt.Println(resp.Header.Get("Content-Type")) //application/json;charset=UTF-8

	type Result struct {
		Msg    string
		Status string
		Obj    string
	}
	result := &Result{}
	json.Unmarshal(body, result) //解析json字符串

	if result.Status == "1" {
		fmt.Println(result.Msg)
	} else {
		fmt.Println("login error")
	}
	fmt.Println(result)
}

func main2() {
    client := &http.Client{}
    req, err := http.NewRequest("GET", "http://www.baidu.com", nil)
    if err != nil {
      log.Println(err)
      return
    }
    req.Header.Set("Nam", "name")
    req.Header.Set("ua", "go_comp")
    req.Header.Set("other", "other")

    resp, err := client.Do(req)
    defer resp.Body.Close()

    body, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        //error
        return
    }
	//fmt.Println(reflect.TypeOf(resp.Body)) //看body类型 *http.gzipReader 
    fmt.Println(string(body))
}

func main1() {
    resp, err := http.Get("http://www.baidu.com")
    if err != nil {
        // handle error
    }
    defer resp.Body.Close()
    body, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        // handle error
    }
    fmt.Println(string(body))
}
