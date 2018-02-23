package main

import (
	"fmt"
	//"runtime"
	"sync"
)

func main() {
	//runtime.GOMAXPROCS(1)
	wg := sync.WaitGroup{}
	wg.Add(20) //+20
	for i := 0; i < 10; i++ {
		go func() {
			fmt.Println("i: ", i)
			wg.Done() //-1
		}()
	}
	for i := 0; i < 10; i++ {
		go func(i int) {
			fmt.Println("i: ", i)
			wg.Done() //-1
		}(i)
	}
	wg.Wait() //阻塞等待wg为0才退出函数
}

func main2() {
    chs := make([]chan int, 20)
	for i := 0; i < 10; i++ {
        chs[i] = make(chan int)
		go func(ch chan int) {
			fmt.Println("1 i: ", i)
			ch <- i
		}(chs[i])
	} //都是遍历结束后才执行func

	for i := 0; i < 10; i++ {
	    chs[10 + i] = make(chan int)
		go func(ch chan int,i int) {
			fmt.Println("2 i: ", i)
			ch <- i
		}(chs[10 + i],i)
		//fmt.Printf("----\n")
	}

    for _, ch := range chs {
      <-ch
    }
}
