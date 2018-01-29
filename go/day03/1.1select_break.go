package	main

import (
	"fmt"
	"time"
)

func test() {
    i := 0
    for {
        select {
        case <-time.After(time.Second * time.Duration(2)): //select 2s超时
            i++
            if i == 5 {
                fmt.Println("break now")
                break //只break出select 仍在for循环里
            }
            fmt.Println("inside the select: ")
        }
        fmt.Println("for: ")
    }
}

func test1() {
	i := 0
	ForEnd:
	for {
		select {
		case <-time.After(time.Second * time.Duration(2)):
			i++
			if i == 5 {
				fmt.Println("break now")
				break ForEnd //竟然结束了
			}
			fmt.Println("inside the select: ")
		}
		fmt.Println("for: ")
	}
}

func test2() {
	i := 0

	for {
		select {
		case <-time.After(time.Second * time.Duration(2)):
			i++
			if i == 5 {
				fmt.Println("break now")
				goto ForEnd
			}
			fmt.Println("inside the select: ")
		}
		fmt.Println("inside the for: ")
	}
	ForEnd：
}

func main() {
	//test()
	//test1()
	test2()
}
