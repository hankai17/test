timeout := make(chan bool, 1)
go func() {
    time.Sleep(1 * time.Second)
    timeout <- true
}()

select {
    case <-ch:
    case <-timeout:
}

//为什么不用make(chan bool) 为什么要留一个缓冲区
