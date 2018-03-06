package main
import (
  "fmt"
)


//make用于channel、map和slice的创建并初始化零值。new是指针对象
func main() {
  //var p *[]int = new([]int)       // p=nil, 没有任何用处，只是分配了一个指针对象
  var p  []int = make([]int, 100) // 而make则分配了长度和容量等于100，并且初始化零值的对象
  fmt.Println(p)

  fmt.Println("------------")

  // 复杂方法:
  var q *[]int = new([]int)
  //*q = make([]int, 100, 100)
  *q = make([]int, 100 )
  fmt.Println(q)
}
