package main

import (
	"fmt"
)

type handler_func func(k, v interface{}) //函数对象
func (f handler_func) Do(k, v interface{}){ //函数对象的一个方法
      f(k,v)
}

func Each(m map[interface{}]interface{}, h handler_func) {
    if m != nil && len(m) > 0 {
        for k, v := range m {
            h.Do(k, v)
        }
    }
}

type welcome string
func (w welcome) selfInfo(k, v interface{}) { //实现函数对象
    fmt.Printf("%s,我叫%s,今年%d岁\n", w,k, v)
}

func main() {
    persons := make(map[interface{}]interface{})
    persons["张三"] = 20
    persons["李四"] = 23
    persons["王五"] = 26

    var w welcome = "大家好"

    Each(persons, handler_func(w.selfInfo))
}
