package main
import (
  "fmt"
)
/*
func main() {
	i := GetValue()

	//switch i.(type) { //error type只能用于interface
	switch i.(type) {
	case int:
		println("int")
	case string:
		println("string")
	case interface{}:
		println("interface")
	default:
		println("unknown")
	}
}
*/

func GetValue() int {
	return 1
}

func GetValue1() interface{} {
    var i interface{} = 1
	return i
}


func main() {
	//var i interface{} = "TT"
	var i interface{} = 77

	t := i.type
	fmt.Println(t)

	value, ok := i.(int)
	if ok {
		fmt.Printf("类型匹配int:%d\n", value)
	} else {
		fmt.Println("类型不匹配int\n")
	}

	if value, ok := i.(int); ok {
		fmt.Println("类型匹配整型：%d\n", value)
	} else if value, ok := i.(string); ok {
		fmt.Printf("类型匹配字符串:%s\n", value)
	}
}
