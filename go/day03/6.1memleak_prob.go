package main

import (
  "fmt"
)

//从replicas数组的各个元素中找关键字query 每个数组起一个go协程
func First(query string, replicas ...Search) Result {
    c := make(chan Result) //这意味着只有第一个goroutine返回 其他的goroutine会困在尝试发送结果的过程中 这意味着泄露资源
    //解决方案 c := make(chan Result, len(replicas)) 保存所有结果 ------这个方案5000
    searchReplica := func(i int) { c <- replicas[i](query) }
    for i := range replicas {
        go searchReplica(i)
    }
    return <-c
}

//防止内存泄漏的两个方案
//这个方案7000
func First(query string, replicas ...Search) Result {
    c := make(chan Result, 1)
    searchReplica := func(i int) {
        select {
        case c <- replicas[i](query):
        default:
        }
    }
    for i := range replicas {
        go searchReplica(i)
    }
    return <-c
}

//这个方案7000
func First(query string, replicas ...Search) Result {
    c := make(chan Result)
    done := make(chan struct{})
    defer close(done)
    searchReplica := func(i int) {
        select {
        case c <- replicas[i](query):
        case <- done:
        }
    }
    for i := range replicas {
        go searchReplica(i)
    }

    return <-c
}
//https://www.colabug.com/612936.html 内存泄漏解决的方案
