package main
import (
	"fmt"
)

//http://blog.csdn.net/kjfcpua/article/details/18265441

//如果信道正有数据在流动，我们还要加入数据，或者信道干涩，我们一直向无数据流入的空信道取数据呢？ 就会引起死锁
//死锁经典案例
func main() {
    ch := make(chan int)
    <-ch // 阻塞main goroutine, 信道c被锁
}

//案例2
var ch1 chan int = make(chan int)
var ch2 chan int = make(chan int)
func say(s string) {
    fmt.Println(s)
    ch1 <- <- ch2 // ch1 等待 ch2流出的数据
}

//案例3
c, quit := make(chan int), make(chan int)
go func() {
   c <- 1  // c通道的数据没有被其他goroutine读取走，堵塞当前goroutine
   quit <- 0 // quit始终没有办法写入数据
}()
<-quit // quit 等待数据的写
func main() {
    go say("hello")
    <-ch1  // 堵塞主线
}

//死锁原因 非缓冲信道上如果发生了流入无流出，或者流出无流入，也就导致了死锁
//因为无缓冲信道不能承载数据，那么就赶紧拿走

