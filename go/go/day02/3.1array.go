package main
import (
        "fmt"
       )

func main() {
    //array [2][3]int 
    array := {{1,1,1},{1,1,1}}
    for i := 0; i< len(array); i++ {
        fmt.Println("element", i, "of array is", array[i])
    }
}
