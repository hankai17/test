package main
import "fmt"

func main()  {
    type MyInt1 int //define
    type MyInt2 = int //alias

    var i int =9
    var i1 MyInt1 = i //不能直接赋值 得强转
    var i2 MyInt2 = i

    fmt.Println(i1,i2)
}
//有问题？？？？
