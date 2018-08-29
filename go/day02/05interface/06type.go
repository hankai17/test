package main
import (
    "errors"
    "fmt"
    "reflect"
)

func Call(m map[string]interface{}, name string, params ...interface{}) (result []reflect.Value, err error) {
    f := reflect.ValueOf(m[name])
    if len(params) != f.Type().NumIn() {
        err = errors.New("The number of params is not adapted.")
        return
    }
    in := make([]reflect.Value, len(params)) //又见reflect.Value
    for k, param := range params {
        in[k] = reflect.ValueOf(param)
    }
    result = f.Call(in)
    fmt.Println("result:", result)
    return
}

type AgentLogin struct {
    UserName   string
    Password   string
    RePassword string
}

func AgentLoginFunc(msg *AgentLogin) (int, string) {
    return 3, "abc"
}

var AgentFuncs map[string]interface{} = map[string]interface{} { "AgentLogin": AgentLoginFunc, }

func main() {
    al := &AgentLogin{"mzy", "123", "123"}
    r, _ := Call(AgentFuncs, "AgentLogin", al)
    fmt.Printf("r:%v\n", r)
    fmt.Println(r[0].Int())
    fmt.Println(r[1].String())
}

//result: [<int Value> abc]
//r:[<int Value> abc]

