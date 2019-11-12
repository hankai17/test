package main

import "sync"

/*
type myMutex sync.Mutex
func main() {
    var mtx myMutex
    mtx.Lock() //error //(type myMutex has no field or method Lock)
    mtx.Unlock() //error  
}
//把一个现有(非interface)的类型定义为一个新的类型时 新的类型不会继承现有类型的方法
*/

/*
type myLocker struct { //用匿名方式把原有类型嵌入/代理其中  //Go语言中的继承是通过内嵌或组合来实现的
    sync.Mutex //看golang包 mutex是个结构体
} 
func main() {
    var lock myLocker
    lock.Lock() //ok //把一个元素包装成struct的好处 最大的好处就是继承!!!!!!!!!!!!!
    lock.Unlock() //ok
}
*/

type myLocker sync.Locker //Locker是接口
func main() {
    var lock myLocker = new(sync.Mutex)
    lock.Lock() //ok
    lock.Unlock() //ok
}


