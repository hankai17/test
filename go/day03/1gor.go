package main

import "fmt"

func Add(x, y int) {
	z := x + y
	fmt.Println(z)
}

func main() {
	for i := 0; i < 10; i++ {
		go Add(i, i) //一旦将go放在函数之前 意味分配一个子routine让这个函数自个玩去
	}
}
//由于没有wait 类似的函数调用所以函数立即返回
//引入通信的概念

//那么问题来了 这么做确实解决了主routine不被锁死的问题 又能够完成了耗时计算 可是计算出答案之后 如何回传给主routine使用呢
//这也就是下面channel的使用了
//goroutine 之间如何进行数据的通信 如下两种 1共用内存内存空间 2Go语言推荐的通信机制channel
