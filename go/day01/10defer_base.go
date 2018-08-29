package main

import (
  "fmt"
  "os"
  "path/filepath"
)

func main1() {
    var i int = 1
    defer fmt.Println("result =>",func() int { return i * 2 }())
    i++
    //prints: result => 2 (not ok if you expected 4)
}

func main() {
    if len(os.Args) != 2 {
        os.Exit(-1)
    }

    start, err := os.Stat(os.Args[1]) // ./
    if err != nil || !start.IsDir() {
        os.Exit(-1)
    }

    var targets []string
    fmt.Println("-----------------")
    filepath.Walk(os.Args[1], func(fpath string, fi os.FileInfo, err error) error {
        if err != nil {
            return err
        }

        if !fi.Mode().IsRegular() {
            return nil
        }

        targets = append(targets, fpath) //把当前目录下所有文件全都append到slice中  这好像是个递归lambuda
        fmt.Println(fpath)
        return nil
    })
    fmt.Println("-----------------")

    for _, target := range targets { //遍历所有文件
        f, err := os.Open(target)
        if err != nil {
            fmt.Println("bad target:",target,"error:",err) //prints error: too many open files
            break
        }
        fmt.Println("open file",target)
        defer f.Close() //will not be closed at the end of this code block //defer是在函数末尾执行 而非代码块结束执行
        //do something with the file...
    }
    //而是在这里close
}

/*
解决方法 写成闭包函数
	for _, target := range targets {
        func() {
            f, err := os.Open(target)
            if err != nil {
                fmt.Println("bad target:",target,"error:",err)
                return
            }
            defer f.Close() //ok
            //do something with the file...
        }()
    }
*/
