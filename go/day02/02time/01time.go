package main

import (
	"fmt"
	"time"
)


func main() {
	//closeChannel()
	c := make(chan int) //一个管道
	timeout1 := time.After(time.Second * 2) //2s超时管道
	timeout2 := time.NewTimer(time.Second * 3) //3s超时
	var i int
	go func() {
		for { //协程大循环
			select {
			case <-c:
				fmt.Println("channel sign")
				return //退出协程
			case <-timeout2.C: //3s超时
				fmt.Println("3s超时到")
			case <-timeout1:
				i++
				fmt.Println(i, "2s超时到")
			case <-time.After(time.Second * 4): //是本次监听动作的超时时间， 意思就说，只有在本次select 操作中会有效， 再次select 又会重新开始计时（从当前时间+4秒后）， 但是有default ，那case 超时操作，肯定执行不到了。
				fmt.Println("4s超时")
			default:
				fmt.Println("default sleep 1s")
				time.Sleep(time.Second * 1)
			}
		}
	}()
	time.Sleep(time.Second * 6)
	close(c)
	time.Sleep(time.Second * 2)
	fmt.Println("main退出")
}
