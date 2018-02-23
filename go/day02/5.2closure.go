package main

import (
	"fmt"
)

func main() {
	var j int = 5
	
	a := func()(func()) { //相当于一个对象
		var i int = 10
		return func() {
		    i += 1
		    j += 1
			fmt.Printf("i, j: %d, %d\n", i, j);
		}
	}()

	a() //11 6
	j *= 2 
	a() //12 13
}
