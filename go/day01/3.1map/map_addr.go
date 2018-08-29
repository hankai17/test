package main

import (
	"fmt"
  _ "reflect"
)
type notifier interface {
    notify()
}

type user struct {
    name  string
    email string
}
func (u *user) notify() {
    fmt.Printf("Sending user email to %s",u.name)
}

func sendNotification(n notifier) {
     n.notify()
}

func main1() {
    u := user{"miki", "dog@email.com"}
    //sendNotification(u)
    sendNotification(&u)
}

/*
./interface.go:24: cannot use u (type user) as type notifier in argument to sendNotification:
	user does not implement notifier (notify method has pointer receiver)
*/
///////////////////////////////////////////////////////////

type data struct {
    name string
}
func (p *data) print() { //Go方法是作用在接收者receiver上的一个函数 这里的接受者就是p  接收者是某种类型的变量 因此方法是一种特殊类型的函数
    fmt.Println("name:", p.name)
}

type printer interface {
    print()
}

func main() {
    d1 := data{"one"} //d1就是选择器 通过选择器来引用结构体字段
    d1.print() //ok

    //var in printer = data{"two"} //error
    var in printer = &data{"two"} //ok
    in.print()

    n := map[string]data {"x": data{"three"}}  //并不是所有的变量是可取址的 Map的元素就不是 通过interface引用的变量也不是
    fmt.Println(&n["x"]) //cannot take the address of n["x"]
    n["x"].print() //so this is error

    //m := map[string]*data {"x": &data{"three"}}
    //m["x"].print() //ok
}

//而slice是可以寻址的
type data struct {
    name string
}

func main0() {
    //m := map[string]data {"x": {"one"}}
    //m["x"].name = "two" //error map中的元素不能取地址

	s := []data {{"one"}}
    s[0].name = "two" //ok //slice可以取地址
    fmt.Println(s)    //prints: [{two}]
}

func main() {
    m := map[string]data {"x": {"one"}}
    r := m["x"] //解决方案是临时值
    r.name = "two"
    m["x"] = r
    fmt.Printf("%v", m) //prints: map[x:{two}]
}

func main2() {
    m := map[string]*data {"x": {"one"}}
    m["x"].name = "two" //ok
    fmt.Println(m["x"]) //prints: &{two}
}
