package main
import (
  //"strings"
  "fmt"
)

type Stringer interface {
    String() string
}

type S struct {
     i int
}
func (s *S) String() string {
    return fmt.Sprintf("%d", s.i)
}

func Print(s Stringer) {
    println(s.String())
}
func DynamicPrint(any interface{}) {
   if s, ok := any.(Stringer); ok {
       Print(s)
   }
}

func main1() {
   var s S
   s.i = 123456789
   Print(&s) //Print(s) err
   DynamicPrint(&s)
}

func Any(v interface{})  {
    if v2, ok := v.(string);ok{
        println(v2)
    }else if v3,ok2:=v.(int);ok2{
        println(v3)
    }
}

func main() {
    Any(2)
    Any("666")
}

/*
go不是一种典型的OO语言 语法上不支持类和继承 interface + methods实现了多态
An interface type specifies a method set called its interface. A variable of interface type can store a value of any type with a method set that is any superset of the interface. Such a type is said to implement the interface. The value of an uninitialized variable of interface type is nil.
interface定义了一个或一组method(s) 这些method(s)只有函数签名没有具体实现(虚函数)
潜规则: Go中约定的interface类型名通常取其内部声明的method名的er形式
*/
