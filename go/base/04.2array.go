package main

import "fmt"

func main2() {
    h, w := 2, 4

    raw := make([]int,h*w)
    for i := range raw {
        raw[i] = i
    }
    fmt.Println(raw,&raw[4])
    //prints: [0 1 2 3 4 5 6 7] <ptr_addr_x>

    table := make([][]int,h)
    for i:= range table {
        table[i] = raw[i*w:i*w + w]
    }

    //fmt.Println(table,&table[1][0])
    fmt.Println(table)
    //prints: [[0 1 2 3] [4 5 6 7]] <ptr_addr_x>
}

func main() {
    x := 2
    y := 4

    table := make([][]int,x) //2个元素
    for i:= range table {
        table[i] = make([]int,y) //每个元素包含4个
    }
    fmt.Println(table)
}
