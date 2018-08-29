package main

import (
  "fmt"
  "errors"
)

func testPanic2Error() (err error) {
  defer func() {
	if r := recover(); r != nil {
	  fmt.Println("Recovered in testPanic2Error", r)
	  //check exactly what the panic was and create error.
	  switch x := r.(type) {
	  case string:
		err = errors.New(x)
	  case error:
		err = x
	  default:
		err = errors.New("Unknow panic")
	  }
	}
  }()

  //logic code , panic here.
  //panic("i am string")
  //panic(errors.New("i am error"))
  panic(-1)
  return nil
}

func main() {
  fmt.Println(testPanic2Error())
  fmt.Println("panic restore now, continue.")
}

/*
Recovered in testPanic2Error -1
Unknow panic
panic restore now, continue.
*/
