package main
import (
	"fmt"
	//"runtime"
)

func main() {
	//runtime.GOMAXPROCS(1)
	int_chan := make(chan int, 1)
	string_chan := make(chan string, 1)

	int_chan <- 1
	string_chan <- "hello"

	select { //随机算法
	case value := <-int_chan:
		fmt.Println(value)
	case value := <-string_chan:
		panic(value)
	}
}
