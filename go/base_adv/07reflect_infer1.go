package main

import (
  "fmt"
  "reflect"
  "log"
)
//看来要是想在返回值也用接口 还是比较麻烦的
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
  squared := Map1(func(x string) int { return len(x) }, []int{1, 2, 3}).([]int)
  fmt.Println(squared)
}

func Map1(f interface{}, xs interface{}) interface{} {
  vf := reflect.ValueOf(f)
  vxs := reflect.ValueOf(xs)

  ftype := vf.Type()
  xstype := vxs.Type()

  // 1) Map's first parameter type must be `func(A) B`
  if ftype.Kind() != reflect.Func {
    log.Panicf("`f` should be %s but got %s", reflect.Func, ftype.Kind())
  }
  if ftype.NumIn() != 1 {
    log.Panicf("`f` should have 1 parameter but it has %d parameters",
      ftype.NumIn())
  }
  if ftype.NumOut() != 1 {
    log.Panicf("`f` should return 1 value but it returns %d values",
      ftype.NumOut())
  }

  // 2) Map's second parameter type must be `[]A1` where `A == A1`.
  if xstype.Kind() != reflect.Slice {
    log.Panicf("`xs` should be %s but got %s", reflect.Slice, xstype.Kind())
  }
  if xstype.Elem() != ftype.In(0) {
    log.Panicf("type of `f`'s parameter should be %s but xs contains %s",
      ftype.In(0), xstype.Elem())
  }

  // 3) Map's return type must be `[]B1` where `B == B1`.
  tys := reflect.SliceOf(vf.Type().Out(0))

  vys := reflect.MakeSlice(tys, vxs.Len(), vxs.Len())
  for i := 0; i < vxs.Len(); i++ {
    y := vf.Call([]reflect.Value{vxs.Index(i)})[0]
    vys.Index(i).Set(y)
  }
  return vys.Interface()
}
