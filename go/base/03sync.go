package main

import (
    "fmt"
    "sync"
)

func main() {
    var wg sync.WaitGroup
    done := make(chan struct{})
    workerCount := 2

    for i := 0; i < workerCount; i++ {
        wg.Add(1)
        go doit(i, done, wg)
    }

    close(done)
    wg.Wait()
    fmt.Println("all done!")
}

func doit(workerId int, done <-chan struct{}, wg sync.WaitGroup) {
    fmt.Printf("[%v] is running\n",workerId)
    defer wg.Done()
    <-done
    fmt.Printf("[%v] is done\n",workerId)
}
//死锁发生是因为各个worker都得到了原始的“WaitGroup”变量的一个拷贝。当worker执行 wg.Done()时，并没有在主goroutine上的“WaitGroup”变量上生效。


