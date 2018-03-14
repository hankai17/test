package main

import (
  "fmt"
  "reflect"
)
//https://blog.burntsushi.net/type-parametric-functions-golang/
type Foo struct {
  X string
  Y int
}

func (f Foo) Do() {
  fmt.Printf("X is: %s, Y is: %d\n", f.X, f.Y)

}

func main1() {
  var foo = &Foo{"abc", 123}
  reflect.ValueOf(foo).MethodByName("Do").Call([]reflect.Value{})

}

func Map(f interface{}, xs interface{}) interface{} {
  vf := reflect.ValueOf(f)
  vxs := reflect.ValueOf(xs)

  tys := reflect.SliceOf(vf.Type().Out(0))
  vys := reflect.MakeSlice(tys, vxs.Len(), vxs.Len())

  for i := 0; i < vxs.Len(); i++ {
    y := vf.Call([]reflect.Value{vxs.Index(i)})[0]
    vys.Index(i).Set(y)
  }
  return vys.Interface()
}


func main() {
  //squared := Map(func(x int) int { return x * x }, []int{1, 2, 3}).([]int)
  squared := Map(func(x int) int { return x * x }, []int{1, 2, 3})
  fmt.Println(reflect.TypeOf(squared))

  _, flag := squared.([]int)
  fmt.Println("flag =",flag)

  _, flag1 := squared.(string)
  fmt.Println("flag1 =",flag1)


  /*
  square := func(x int) int {
	return x * x
  }
  nums := []int{1, 2, 3, 4}
  gsquared := Map(square, nums)
  fmt.Println(gsquared)
  */
}

