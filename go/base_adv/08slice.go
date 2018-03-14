package main
import "fmt"

func main() {
    s1 := []int{1,2,3}
    fmt.Println(len(s1), cap(s1),s1) //prints 3 3 [1 2 3]

    s2 := s1[1:]
    fmt.Println(len(s2), cap(s2),s2) //prints 2 2 [2 3]

    for i := range s2 { s2[i] += 20 }

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
