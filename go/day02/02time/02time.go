package main

import (
	"fmt"
	"time"
)

//发送者
func sender(c chan int) {
	for i := 0; i < 100; i++ {
		c <- i
		if i >= 5 {
			time.Sleep(time.Second * 7)
		} else {
			time.Sleep(time.Second)
		}
	}
}

func main() {
	c := make(chan int)
	go sender(c)
	timeout := time.After(time.Second * 3)
	for {
		select {
		case d := <-c:
			fmt.Println(d) //0--4
		case <-timeout:
			fmt.Println("定时3s到")
		case dd := <-time.After(time.Second * 3): //select的超时
			fmt.Println(dd, "select的超时")
		}
		fmt.Println("for end")
	}
}
