package main

import (
  "fmt"
  "time"
  "strconv"
  "runtime"
  "net/http"
 _ "net/http/pprof"
)

func main1() {
	ch := make(chan int)
	for i := 0; i < 3; i++ {
		go func() {
			time.Sleep(time.Second * time.Duration(1))
			ch <- 1 //xs后返回
		}()
	}

	timeout1 := time.After(time.Second * 2)
    select {
        case ret := <-ch:
            fmt.Println("ret =", ret)
        case  <-timeout1:
            return
    }
	close(ch)
	time.Sleep(time.Second * time.Duration(1))
}
//1向关闭的ch发送信息报错
//内存泄漏是内存泄漏 gor没有正常退出就是内存泄漏 跟关不关ch没有半毛钱关系 

func test_mem_leak() {
	//go func() {
    //    http.ListenAndServe("localhost:6060", nil)
    //}() //go tool pprof http://localhost:6060/debug/pprof/heap

	ch := make(chan int)
	for i := 0; i < 3; i++ {
		go func() {
			time.Sleep(time.Second * time.Duration(1))
			ch <- 1 //xs后返回
			fmt.Println("gor 正常退出")
		}()
	}

	timeout1 := time.After(time.Second * 2)
    select {
        case ret := <-ch:
            fmt.Println("ret =", ret)
        case  <-timeout1:
            return
    }
	//close(ch)
}

func main() {
	go func() {
        http.HandleFunc("/goroutines", func(w http.ResponseWriter, r *http.Request) {
            num := strconv.FormatInt(int64(runtime.NumGoroutine()), 10)
            w.Write([]byte(num))
        });
        http.ListenAndServe("localhost:6060", nil)
    }() //curl localhost:6060/goroutines 直接看gor的个数
/*
	for {
		test_mem_leak()
		time.Sleep(time.Second * time.Duration(1))
	}
*/
/*
	for {
		go test_mem_leak()
		time.Sleep(time.Second * time.Duration(1))
	}
*/
/*
		go test_mem_leak()
		go test_mem_leak()
		go test_mem_leak()
		go test_mem_leak()
		go test_mem_leak()
*/
	time.Sleep(time.Second * time.Duration(3))
	for i := 0; i < 1; i++ {
		go test_mem_leak()
		//time.Sleep(time.Second * time.Duration(1))
	}
	time.Sleep(time.Second * time.Duration(10))
}
//2内存泄漏测试
/*
5root@hkdomain:go# curl 127.0.0.1:6060/goroutines
5root@hkdomain:go# curl 127.0.0.1:6060/goroutines
5root@hkdomain:go# curl 127.0.0.1:6060/goroutines
9root@hkdomain:go# curl 127.0.0.1:6060/goroutines
9root@hkdomain:go# curl 127.0.0.1:6060/goroutines
7root@hkdomain:go# curl 127.0.0.1:6060/goroutines
7root@hkdomain:go# curl 127.0.0.1:6060/goroutines
7root@hkdomain:go# curl 127.0.0.1:6060/goroutines
7root@hkdomain:go# curl 127.0.0.1:6060/goroutines
7root@hkdomain:go# curl 127.0.0.1:6060/goroutines
*/
//看从5 到最后的 7 增加了两个 也就是说那两个gor没有释放
//gor无法正常退出就意味着内存泄漏
