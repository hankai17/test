package main

import (
  "fmt"
  "time"
  //"strconv"
  //"runtime"
  //"net/http"
// _ "net/http/pprof"
)
func test_mem_leak() {
	ch := make(chan int)
	done := make(chan struct{}) //方案一 引入一个done管道
	//defer close(done) //确保这个函数结束后 向gors发送信号 释放内存

	for i := 0; i < 2; i++ {
		go func() {
			time.Sleep(time.Second * time.Duration(1))
			select {
				case ch <- 1: //xs后返回
					fmt.Println("gor return val")
				case <- done:
					fmt.Println("gor done")
			}
		}()
	}

	timeout1 := time.After(time.Second * 2)
    select {
        case ret := <-ch:
            fmt.Println("ret =", ret)
        case  <-timeout1:
            return
    }
	close(done)
	time.Sleep(time.Second * time.Duration(1))
	defer close(ch) //习惯 跟内存泄漏没有关系 //当然也可以不关
			  //关闭管道意味着: 1我不读数据了 2gors不能发数据了
			  //那么问题来了 gors为什么不能发数据了? gors结束了
			  //也就是说调用close(ch)是有很沉重的代价的 必须确保gors不能向我发数据了 必须确保gors结束了
			  //so 必须手动写close(done) 而不能defer
			  //so 关闭ch可以写defer  ch关闭的越晚越好  不知道defer有没有顺序 可以让代码再简洁点

	//time.Sleep(time.Second * time.Duration(1))
	//do other
}

func main() {
	test_mem_leak()
}
//https://blog.csdn.net/hittata/article/details/51782441 gor的正确用法
