package main

import "fmt"
import "time"

//向一个nil的channel上发送和接收操作会被永久阻塞 出现死锁问题
func main1() {
    var ch chan int //这就是nil channel 
    //ch := make(chan int)
    for i := 0; i < 3; i++ {
        go func(idx int) {
            ch <- (idx + 1) * 2
        }(i)
    }

    //get first result
    fmt.Println("result:",<-ch)
    //do other work
    time.Sleep(2 * time.Second)
}

func main() { //没看懂
    inch := make(chan int)
    outch := make(chan int)

    go func() {
        var in <-chan int = inch
        var out chan<- int
        var val int
        for {
           select {
            case out <- val:
                out = nil
                in = inch
            case val = <-in:
                out = outch
                in = nil
            }
        }
    }()

    go func() {
        for r := range outch {
            fmt.Println("result:",r)
        }
    }()

    time.Sleep(0)
    inch <- 1
    inch <- 2
    time.Sleep(3 * time.Second)
}
