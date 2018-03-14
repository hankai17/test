package main

import (
  "fmt"
  "reflect"
)
func Map(f func(int) int, xs []int) []int {
    ys := make([]int, len(xs))
    for i, x := range xs {
        ys[i] = f(x)
    }
    return ys
}

func main1() {
	ret := Map(func(x int) int { return x * x }, []int{1, 2, 3})
	fmt.Println(ret) //1 4 9
}

/*
func Map(f func(A) B, xs []A) []B {
    ys := make([]B, len(xs))
    for i, x := range xs {
        ys[i] = f(x)
    }
    return ys
}
运行时候 要求map函数可根据传入的类型 自动推到 即是重载	
*/

//尝试用万能的接口类型 //interface可以转为任何类型
func Map1(f func(interface{}) interface{}, xs []interface{}) []interface{} {
  ys := make([]interface{}, len(xs))
  for i, x := range xs {
    ys[i] = f(x)
  }
  return ys
}

func main2() {
	//ret := Map1(func(x int) int { return x * x }, []int{1, 2, 3}) //now map1 cannot use in this way
	square := func(x interface{}) interface{} {
	  return x.(int) * x.(int)
	}
	nums := []int{1, 2, 3, 4}

	gnums := make([]interface{}, len(nums))
	for i, x := range nums {
	  gnums[i] = x
	}

	gsquared := Map1(square, gnums)
	squared := make([]int, len(gsquared))
	for i, x := range gsquared {
	  squared[i] = x.(int)
	}
	fmt.Println(squared)
} //可以实现但是我太麻烦

//接口可以接受任何类型 参数全用接口类型 再在函数内部对此接口进行还原
func Map2(f interface{}, xs interface{}) []interface{} {
//func Map2(f interface{}, xs interface{}) interface{} { //之所以返回值用接口切片 是因为只有用切片时 才可以用range遍历
  vf := reflect.ValueOf(f)
  vxs := reflect.ValueOf(xs)
  ys := make([]interface{}, vxs.Len())

  for i := 0; i < vxs.Len(); i++ {
    ys[i] = vf.Call([]reflect.Value{vxs.Index(i)})[0].Interface()
  }
  return ys
}

func main() {
	square := func(x int) int {
	  return x * x
	}
	nums := []int{1, 2, 3, 4}

	//gsquared := Map1(square, nums) //cannot use square (type func(int) int) as type func(interface {}) interface {}
	gsquared := Map2(square, nums)
	squared := make([]int, len(gsquared))
	//fmt.Println(reflect.ValueOf(gsquared))
	//fmt.Println(reflect.TypeOf(gsquared))
	//fmt.Println(len(reflect.ValueOf(gsquared)))
	//squared := make([]int, (reflect.ValueOf(gsquared)).Len())

	//tmp := reflect.ValueOf(gsquared)
	//fmt.Println( reflect.TypeOf(gsquared) )
	for i, x := range gsquared { //cannot range over gsquared (type interface {})
	  squared[i] = x.(int)
	}
	fmt.Println(gsquared)
} //这样我轻松了不少 而且对于编程人员来说非常合理 我只需把参数穿进去就可以了





