package main

import (
  "fmt"
  "bytes"
)

func get() []byte {
    raw := make([]byte,10000)
   fmt.Println(len(raw),cap(raw),&raw[0]) //prints: 10000 10000 0xc420054000
    return raw[:3]
}

func get1() []byte {
    raw := make([]byte,10000)
    fmt.Println(len(raw),cap(raw),&raw[0]) //prints: 10000 10000 <byte_addr_x>

    res := make([]byte,3)
    copy(res,raw[:3])
    return res // 3 3 地址变化了
}

func main() {
    data := get1()
    fmt.Println(len(data),cap(data),&data[0]) //prints: 3 10000 0xc420054000
}



func main1() {
    s1 := []int{1,2,3}
    fmt.Println(len(s1), cap(s1), s1) //prints 3 3 [1 2 3]

    s2 := s1[1:]
    fmt.Println(len(s2), cap(s2), s2) //prints 2 2 [2 3]

    for i := range s2 {
		s2[i] += 20
	}

    //still referencing the same array
    fmt.Println(s1) //prints [1 22 23]
    fmt.Println(s2) //prints [22 23]

    fmt.Println("before append addr = ", &s2[0])
    s2 = append(s2, 4) //append操作可能会有复制操作
    fmt.Println("after append addr = ", &s2[0])

    for i := range s2 { s2[i] += 10 }

    //s1 is now "stale"
    fmt.Println(s1) //prints [1 22 23]
    fmt.Println(s2) //prints [32 33 14]
}

func main() {
    path := []byte("AAAA/BBBBBBBBB")
    sepIndex := bytes.IndexByte(path, '/')
    dir1 := path[:sepIndex]
    //dir1 := path[:sepIndex:sepIndex] //full slice expression //cap=4即指定容量 当发生append操作时候将重新分配内存
    dir2 := path[sepIndex+1:] //cap=16
    fmt.Println("add =",&dir1[0],"cap =",cap(dir1),"dir1 =>",string(dir1)) //prints: dir1 => AAAA
    fmt.Println("add =",&dir2[0],"cap =",cap(dir2),"dir2 =>",string(dir2)) //prints: dir2 => BBBBBBBBB

    dir1 = append(dir1, "suffix"...) //切片append操作
    path = bytes.Join([][]byte{dir1,dir2}, []byte{'/'})

    fmt.Println("dir1 =>",string(dir1)) //prints: dir1 => AAAAsuffix
    fmt.Println("dir2 =>",string(dir2)) //prints: dir2 => uffixBBBB (not ok)

    fmt.Println("new path =>",string(path)) //AAAAsuffix/uffixBBBB
}

