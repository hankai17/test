package main

import "fmt"

func Add(x, y int) {
	z := x + y
	fmt.Println(z)
}

func main() {
	for i := 0; i < 10; i++ {
		go Add(i, i)
	}
}
//由于没有wait 类似的函数调用所以函数立即返回
//引入通信的概念
