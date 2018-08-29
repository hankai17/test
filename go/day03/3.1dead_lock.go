package main
import (
	"fmt"
)

func main() {
	//var ch chan int //一般chan不这样用
	//ch := make(chan int)
	ch := make(chan int, 1)

	//go func() { //ok
	func() {
		fmt.Println("111111")
		ch <- 1
	}()
		fmt.Println("211111")

	fmt.Printf("recv %d\n", <-ch)
}
//顺序问题:闭包又不是goroutine 考虑什么顺序问题 傻吊

//死锁经典案例
func main() {
    ch := make(chan int)
    <-ch //阻塞main goroutine 信道c被锁
}

//案例2
var ch1 chan int = make(chan int)
var ch2 chan int = make(chan int)
func say(s string) {
    fmt.Println(s)
    ch1 <- <- ch2 //ch1等待ch2流出的数据
}
func main() {
    go say("hello")
    <-ch1  //堵塞主线
}

//案例3
c, quit := make(chan int), make(chan int)
go func() {
   c <- 1  // c通道的数据没有被其他goroutine读取走 堵塞当前goroutine
   quit <- 0 //quit始终没有办法写入数据
}()
<-quit //quit等待数据的写

//死锁原因 非缓冲信道上如果发生了流入无流出或者流出无流入也就导致了死锁
//因为无缓冲信道不能承载数据那么就赶紧拿走

//http://blog.csdn.net/kjfcpua/article/details/18265441
