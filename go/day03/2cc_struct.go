package main

import (
    "fmt"
    "math/rand"
    "os"
    "runtime"
    "sync"
    "sync/atomic"
    "time"
)

type Scenario struct {
    Name        string
    Description []string
    Examples    []string
    RunExample  func()
}

var s1 = &Scenario {
    Name: "s1",
    Description: []string {
        "简单并发执行任务",
    },
    Examples: []string{
        "比如并发的请求后端某个接口",
    },
    RunExample: RunScenario1,
}

var s2 = &Scenario{
    Name: "s2",
    Description: []string{
        "持续一定时间的高并发模型",
    },
    Examples: []string{
        "在规定时间内，持续的高并发请求后端服务， 防止服务死循环",
    },
    RunExample: RunScenario2,
}

var s3 = &Scenario{
    Name: "s3",
    Description: []string{
        "基于大数据量的并发任务模型, goroutine worker pool",
    },
    Examples: []string{
        "比如技术支持要给某个客户删除几个TB/GB的文件",
    },
    RunExample: RunScenario3,
}

var s4 = &Scenario{
    Name: "s4",
    Description: []string{
        "等待异步任务执行结果(goroutine+select+channel)",
    },
    Examples: []string{
        "",
    },
    RunExample: RunScenario4,
}

var s5 = &Scenario{
    Name: "s5",
    Description: []string{
        "定时的反馈结果(Ticker)",
    },
    Examples: []string{
        "比如测试上传接口的性能，要实时给出指标: 吞吐率，IOPS,成功率等",
    },
    RunExample: RunScenario5,
}

var Scenarios []*Scenario

func init() {
    Scenarios = append(Scenarios, s1)
    Scenarios = append(Scenarios, s2)
    Scenarios = append(Scenarios, s3)
    Scenarios = append(Scenarios, s4)
    Scenarios = append(Scenarios, s5)
}

//常用的并发与同步场景
func main() {
    if len(os.Args) == 1 {
        fmt.Println("请选择使用场景 ==> ")
        for _, sc := range Scenarios {
            fmt.Printf("场景:%s,", sc.Name)
            printDescription(sc.Description)
        }
        return
    }
    for _, arg := range os.Args[1:] {
        sc := matchScenario(arg)
        if sc != nil {
            printDescription(sc.Description)
            printExamples(sc.Examples)
            sc.RunExample()
        }
    }
}

func printDescription(str []string) {
    fmt.Printf("场景描述: %s \n", str)
}

func printExamples(str []string) {
    fmt.Printf("场景举例: %s \n", str)
}

func matchScenario(name string) *Scenario {
    for _, sc := range Scenarios {
        if sc.Name == name {
            return sc
        }
    }
    return nil
}

var doSomething = func(i int) string {
    time.Sleep(time.Millisecond * time.Duration(10))
    fmt.Printf("Goroutine %d do things .... \n", i)
    return fmt.Sprintf("Goroutine %d", i)
}

var takeSomthing = func(res string) string {
    time.Sleep(time.Millisecond * time.Duration(10))
    tmp := fmt.Sprintf("Take result from %s.... \n", res)
    fmt.Println(tmp)
    return tmp
}

//场景1:简单并发任务 
func RunScenario1() { //不需要通信 子协程只管做自己的事情 不需要管道 只需要wait不让主进程退出即可
    count := 10
    var wg sync.WaitGroup
    for i := 0; i < count; i++ {
        wg.Add(1)
        go func(index int) {
            defer wg.Done()
            doSomething(index)
        }(i)
    }
    wg.Wait()
}

//场景2:按时间来持续并发
func RunScenario2() {
    timeout := time.Now().Add(time.Second * time.Duration(10))
    n := runtime.NumCPU()

    waitForAll := make(chan struct{})
    done := make(chan struct{})
    concurrentCount := make(chan struct{}, n)

    for i := 0; i < n; i++ {
        concurrentCount <- struct{}{} //瞬间塞满缓冲管道
    }

    go func() {
        for time.Now().Before(timeout) { //相当于一个生产者 即在一定时间内 收到done信号 则往管道写东西 如果超时则往wait中写东西
            <-done
            concurrentCount <- struct{}{}
        }
        waitForAll <- struct{}{}
    }()

    go func() {
        for {
            <-concurrentCount //相当消费者 一旦缓冲管道有东西 就起协程
            go func() {
                doSomething(rand.Intn(n))
                done <- struct{}{}
            }()
        }
    }()

    <-waitForAll
} //画个图是个循环

//场景3:以worker pool方式并发做事/发送请求
func RunScenario3() {
    numOfConcurrency := runtime.NumCPU()
    taskTool := 10
    jobs := make(chan int, taskTool)
    results := make(chan int, taskTool)
    var wg sync.WaitGroup

    // workExample
    workExampleFunc := func(id int, jobs <-chan int, results chan<- int, wg *sync.WaitGroup) {
        defer wg.Done()
        for job := range jobs {
            res := job * 2
            fmt.Printf("Worker %d do things, produce result %d \n", id, res)
            time.Sleep(time.Millisecond * time.Duration(100))
            results <- res
        }
    }

    for i := 0; i < numOfConcurrency; i++ {
        wg.Add(1)
        go workExampleFunc(i, jobs, results, &wg)
    }

    totalTasks := 100

    wg.Add(1)
    go func() {
        defer wg.Done()
        for i := 0; i < totalTasks; i++ {
            n := <-results
            fmt.Printf("Got results %d \n", n)
        }
        close(results)
    }()

    for i := 0; i < totalTasks; i++ {
        jobs <- i
    }
    close(jobs)
    wg.Wait()
}

//场景4:等待异步任务执行结果(goroutine+select+channel)
func RunScenario4() {
    sth := make(chan string)
    result := make(chan string)

    go func() {
        id := rand.Intn(100)
        for {
            sth <- doSomething(id)
        }
    }()

    go func() {
        for {
            result <- takeSomthing(<-sth)
        }
    }()

    select {
    case c := <-result:
        fmt.Printf("Got result %s ", c)
    case <-time.After(time.Duration(30 * time.Second)):
        fmt.Errorf("指定时间内都没有得到结果")
    }
}

var doUploadMock = func() bool {
    time.Sleep(time.Millisecond * time.Duration(100))
    n := rand.Intn(100)
    if n > 50 {
        return true
    } else {
        return false
    }
}

// 场景5: 定时的反馈结果(Ticker)
// 测试上传接口的性能，要实时给出指标: 吞吐率，成功率等

func RunScenario5() {
    totalSize := int64(0)
    totalCount := int64(0)
    totalErr := int64(0)

    concurrencyCount := runtime.NumCPU()
    stop := make(chan struct{})
    fileSizeExample := int64(10)

    timeout := 10 // seconds to stop

    go func() {
        for i := 0; i < concurrencyCount; i++ {
            go func(index int) {
                for {
                    select {
                    case <-stop:
                        return
                    default:
                        break
                    }

                    res := doUploadMock()
                    if res {
                        atomic.AddInt64(&totalCount, 1)
                        atomic.AddInt64(&totalSize, fileSizeExample)
                    } else {
                        atomic.AddInt64(&totalErr, 1)
                    }
                }
            }(i)
        }
    }()

    t := time.NewTicker(time.Second)
    index := 0
    for {
        select {
        case <-t.C:
            index++
            tmpCount := atomic.LoadInt64(&totalCount)
            tmpSize := atomic.LoadInt64(&totalSize)
            tmpErr := atomic.LoadInt64(&totalErr)
            fmt.Printf("吞吐率: %d，成功率: %d \n", tmpSize/int64(index), tmpCount*100/(tmpCount+tmpErr))
            if index > timeout {
                t.Stop()
                close(stop)
                return
            }
        }

    }
}
