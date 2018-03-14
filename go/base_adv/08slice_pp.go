package main

import (
    "fmt"
    "bytes"
)

func main() {
    path := []byte("AAAA/BBBBBBBBB")
    sepIndex := bytes.IndexByte(path,'/')
    dir1 := path[:sepIndex]
    //dir1 := path[:sepIndex:sepIndex] //full slice expression //cap=4
    dir2 := path[sepIndex+1:] //cap=16
    fmt.Println("add =",&dir1[0],"cap =",cap(dir1),"dir1 =>",string(dir1)) //prints: dir1 => AAAA
    fmt.Println("add =",&dir2[0],"cap =",cap(dir2),"dir2 =>",string(dir2)) //prints: dir2 => BBBBBBBBB

    dir1 = append(dir1, "suffix"...) //切片append操作
    path = bytes.Join([][]byte{dir1,dir2}, []byte{'/'})

    fmt.Println("dir1 =>",string(dir1)) //prints: dir1 => AAAAsuffix
    fmt.Println("dir2 =>",string(dir2)) //prints: dir2 => uffixBBBB (not ok)

    fmt.Println("new path =>",string(path))
}

