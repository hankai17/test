package main

import "fmt"

func main() {
	mySlice := make([]int, 5, 10)
	fmt.Println("len(mySlice):", len(mySlice))
	fmt.Println("cap(mySlice):", cap(mySlice))

    mySlicce = append(mySlice, 1, 2, 3) //加上3个元素
    my_slice2 := []int{8, 9, 10}
    mySlice = append(mySlice, my_slice2...)

}
