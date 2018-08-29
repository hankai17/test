package main

import (
    "fmt"
    "time"
	"sync"
)

func doit(workerId int) {
    fmt.Printf("[%v] is running\n",workerId)
    time.Sleep(3 * time.Second)
    fmt.Printf("[%v] is done\n",workerId)
}

func main0() {
    workerCount := 2
    for i := 0; i < workerCount; i++ {
        go doit(i)
    }
	println("11")
    time.Sleep(1 * time.Second)
    fmt.Println("all done!")
}

func main() {
	//runtime.GOMAXPROCS(1)
	wg := sync.WaitGroup{}
	wg.Add(20) //+20
	for i := 0; i < 10; i++ {
		go func() { //并发的执行   什么时候执行?  M选择好P后执行 也就是wait时执行
			fmt.Println("go1 i: ", i) //全是10
			wg.Done() //-1
		}()
	}
	fmt.Println("===========")
	for i := 0; i < 10; i++ {
		go func(i int) {
			fmt.Println("go2 i: ", i)
			wg.Done() //-1
		}(i)
	}
	wg.Wait() //阻塞等待wg为0才退出函数
}
//goroutine调度模型:  M--->P--->G
//每个g运行在虚拟CPU中 每个g都会被一个特定的P(虚拟CPU)选定维护 而M(物理计算资源)每次回挑选一个有效P 然后执行P中的goroutine
//每个P会将自己所维护的goroutine放到一个G队列中 其中就包括了goroutine堆栈信息是否可执行信息等等 默认情况下P的数量与实际物理CPU的数量相等 因此当我们通过循环来创建goroutine时 每个goroutine会被分配到不同的P队列中 而M的数量又不是唯一的 当M随机挑选P时 也就等同随机挑选了goroutine
//我们设定了P = 1 所以所有的goroutine会被绑定到同一个P中

func main2() {
    chs := make([]chan int, 20)
	for i := 0; i < 10; i++ {
        chs[i] = make(chan int)
		go func(ch chan int) {
			fmt.Println("1 i: ", i)
			ch <- i
		}(chs[i])
	} //都是遍历结束后才执行func

	fmt.Printf("----\n")
	for i := 0; i < 10; i++ {
	    chs[10 + i] = make(chan int)
		go func(ch chan int, i int) {
			fmt.Println("2 i: ", i)
			ch <- i
		}(chs[10 + i], i)
		//fmt.Printf("----\n")
	}

    for _, ch := range chs {
      <-ch
    }
}

func doit1(workerId int, done <-chan struct{}, wg sync.WaitGroup) {
    fmt.Printf("[%v] is running\n",workerId)
    defer wg.Done()
    <-done
    fmt.Printf("[%v] is done\n",workerId)
}

func main3() {
    var wg sync.WaitGroup
    done := make(chan struct{})
    workerCount := 2

    for i := 0; i < workerCount; i++ {
        wg.Add(1)
        go doit1(i, done, wg)
    }

    close(done)
    wg.Wait()
    fmt.Println("all done!")
}
//死锁发生是因为各个worker都得到了原始的“WaitGroup”变量的一个拷贝 当worker执行wg.Done()时 并没有在主goroutine上的“WaitGroup”变量上生效
//但是为什么发生死锁呢？

func doit2(workerId int, wq <-chan interface{}, done <-chan struct{}, wg *sync.WaitGroup) {
    fmt.Printf("[%v] is running\n",workerId)
    defer wg.Done()
    for {
        select {
        case m := <-wq:
            fmt.Printf("[%v] m => %v\n",workerId,m)
        case <- done:
            fmt.Printf("[%v] is done\n",workerId)
            return
        }
    }
}

func main4() {
    var wg sync.WaitGroup
    done := make(chan struct{})
    wq := make(chan interface{})
    workerCount := 2

    for i := 0; i < workerCount; i++ {
        wg.Add(1)
        go doit2(i, wq, done, &wg)
    }

    for i := 0; i < workerCount; i++ {
        wq <- i //通过管道向协程传入数据
    }

    close(done) //通过管道向协程发送done信号
    wg.Wait()
    fmt.Println("all done!")
}

