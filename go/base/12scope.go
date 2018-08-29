package main

import (
    "errors"
    "fmt"
)

var ErrDidNotWork = errors.New("did not work")
func tryTheThing() (string,error)  {
    return "",ErrDidNotWork
}

func DoTheThing(reallyDoIt bool) (err error) {
    if reallyDoIt {
        result, err := tryTheThing()
        if err != nil || result != "it worked" {
            err = ErrDidNotWork
        }
    } //err离开作用域就没了
    return err
}

/*
func DoTheThing(reallyDoIt bool) (err error) {
    var result string
    if reallyDoIt {
        result, err = tryTheThing()
        if err != nil || result != "it worked" {
            err = ErrDidNotWork
        }
    }
    return err
}
*/


func main() {
    fmt.Println(DoTheThing(true))
    //fmt.Println(DoTheThing(false))
}
