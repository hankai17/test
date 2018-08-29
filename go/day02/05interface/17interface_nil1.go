package main

import "fmt"

func main1() {
    doit := func(arg int) interface{} {
        var result *struct{} = nil
        if(arg > 0) {
            result = &struct{}{}
        }
        return result
    }

    if res := doit(-1); res != nil {
        fmt.Println("good result:",res) //prints: good result: <nil>
        //'res' is not 'nil', but its value is 'nil'
    }
}

func main() {
    doit := func(arg int) interface{} {
        var result *struct{} = nil

        if(arg > 0) {
            result = &struct{}{}
        } else {
            return nil //return an explicit 'nil'
        }

        return result
    }

    if res := doit(-1); res != nil {
        fmt.Println("good result:",res)
    } else {
        fmt.Println("bad result (res is nil)") //here as expected
    }
}
