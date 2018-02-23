package main

import "fmt"

func main() {
    slice := []int{0, 1, 2, 3}
    myMap := make(map[int]*int)

    for index, value := range slice {
        fmt.Printf("value=%v\n",value)
        fmt.Printf("&value=%v\n",&value) //即range返回的是一个重复的地址
        a := value
        myMap[index] = &a
    }
    fmt.Println("=====new map=====")
    prtMap(myMap)
}

func prtMap(myMap map[int]*int) {
    for key, value := range myMap {
        fmt.Printf("map[%v]=%v\n", key, *value)
    }
}
