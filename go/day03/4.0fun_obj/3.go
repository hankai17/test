package main

import (
  "fmt"
)

type handler_func func(k, v interface{}) //函数对象
//func (f handler_func) Do(k, v interface{}){ //函数对象的一个方法
//      f(k,v)
//}

func Each(m map[interface{}]interface{}, h handler_func) {
    if m != nil && len(m) > 0 {
        for k, v := range m {
            //h.Do(k, v)
            h(k, v)
        }
    }
}

func EachFunc(m map[interface{}]interface{}, f func(k, v interface{})) {
    Each(m, handler_func(f))
}

func selfInfo(k, v interface{}) {
    fmt.Printf("ok,我叫%s,今年%d岁\n", k, v)
}

func main() {
    persons := make(map[interface{}]interface{})
    persons["张三"] = 20
    persons["李四"] = 23
    persons["王五"] = 26

    EachFunc(persons, selfInfo)
}
