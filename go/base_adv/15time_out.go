timeout := make(chan bool, 1)
go func() {
    time.Sleep(1 * time.Second)
    timeout <- true
}()

select {
    case <-ch:
    // a read from ch has occurred
    case <-timeout:
    // the read from ch has timed out
}

//用make chan 1 而非make chan还是有原因的
//timeout 管道的缓冲区空间为 1，因此 timeout 协程将会在发送消息到管道之后退出执行。协程并不知道（也不关心）管道中的值是否被接受。因此，即使 ch 管道先于 timeout 管道返回了，timeout 协程也不会永久等待。timeout 管道最终会被垃圾回收机制回收掉
//即被gc回收了
