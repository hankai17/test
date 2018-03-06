package main
import (
	"fmt"
)
/*
func main() {
	//make(chan string,1)和make(chan string)是不一样的
	//make(chan string,1)中通道的buffer是1
	//这里如果写成make(chan string)会导致阻塞，死锁,因为没有缓存的通道，不管是读还是写都会阻塞
	//两个解决方案，将push数据移动到写成中（使用go语句），或增加缓存,
	//缓存没有达到上限，就不会出现死锁的情况

	//var chan1 chan string
	//chan1 := make(chan string,1)
	chan1 := make(chan string)
	func() {
		fmt.Printf("在协程中，Push数据到通道\n");
		//chan1 <- "GO吧"
		chan1 <- "G"
	}()

	fmt.Printf("从通道中读取到的数据:%v\n", <-chan1)
}
*/

func main() {
	//var ch chan int //一般chan不这样用
	ch := make(chan int)

	//go func() { //ok
	func() { //err
		fmt.Println("111111")
		ch <- 1
	}()

	fmt.Println("211111")
	fmt.Printf("recv %d\n", <-ch)
}
//闭包也是在main这个协程调用的唯一的协程都挂起了

//顺序问题 有闭包时 先走闭包
//有协程时 最后走协程
