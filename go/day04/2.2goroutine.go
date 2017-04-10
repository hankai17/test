package main

import "fmt"
import "sync"
import "runtime"

var counter int = 0

func Count(lock *sync.Mutex) {
	lock.Lock()
	counter++
	fmt.Println(counter) //fmt.Println(z)
	lock.Unlock()
}

func main() {
	lock := &sync.Mutex{}  //初始化一把锁
	for i := 0; i < 10; i++ {
		go Count(lock)  //把锁当参数传递进去
	}
	for {
		lock.Lock()
		c := counter
		lock.Unlock()
		runtime.Gosched() //别读这段代码了  还是为了引出channel概念 
		if c >= 10 {
			break
		}
	}
}
//不要通过共享内存通信 通过通信共享共享内存
