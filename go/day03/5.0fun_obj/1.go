package main

import (
	"fmt"
)

type Handler interface {
    Do(k, v interface{})
}

func Each(m map[interface{}]interface{}, h Handler) {
    if m != nil && len(m) > 0 {
        for k, v := range m {
            h.Do(k, v)
        }
    }
}
//封装一
type my_string struct {
  welcome string
}
func (w my_string) Do(k, v interface{}) { //不修改值 so不用指针传递
    fmt.Printf("%s, 我叫%s,今年%d岁\n", w.welcome, k, v)
}

//封装二
type welcome string
func (w welcome) Do(k, v interface{}) { //不修改值 so不用指针传递
    fmt.Printf("%s,我叫%s,今年%d岁\n", w, k, v)
}

func main() {
    persons := make(map[interface{}]interface{})
    persons["张三"] = 20
    persons["李四"] = 23
    persons["王五"] = 26
    //Each(persons, w)

    var w welcome = "大家好"
    w.Do("mike", 18)

    var w2 my_string = my_string{"ojbk"}
    //w2 := &my_string{"ojbk"}
    w2.Do("dog", 19)
}

//以上实现，主要有两点不太好：
//必实现Handler接口，而且Do这个方法名不能修改，不能定义一个更有意义的名字
//必新定义一个类型，才可以实现Handler接口，才能使用Each函数
