package main

import (
  "fmt"
  "reflect"
)
func Map(f func(int) int, args []int) []int {
    returns := make([]int, len(args))
    for i, x := range args {
        returns[i] = f(x)
    }
    return returns
}

func main1() {
	ret := Map(func(x int) int { return x * x }, []int{1, 2, 3})
	fmt.Println(ret) //1 4 9
}

/*
func Map(f func(A) B, args []A) []B {
    returns := make([]B, len(args))
    for i, x := range args {
        returns[i] = f(x)
    }
    return returns
}
运行时候 要求map函数可根据传入的类型 自动推到 即重载? no
不清楚输入类型的情况下对输入进行操作
感觉是模板的意思 确实就是go实现模板功能
*/

//尝试用万能的接口类型 //interface可以转为任何类型
func Map1(f func(interface{}) interface{}, args []interface{}) []interface{} {
  returns := make([]interface{}, len(args))
  for i, x := range args {
    returns[i] = f(x)
  }
  return returns
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
} //可以实现但必须使用interface类型 而不像main2那么爽

//接口可以接受任何类型 参数全用接口类型 再在函数内部对此接口进行还原
func Map2(f interface{}, args interface{}) []interface{} { //这接口看起来舒服了
//func Map2(f interface{}, args interface{}) interface{} { //之所以返回值用接口切片 是因为只有用切片时 才可以用range遍历
  vf := reflect.ValueOf(f) ////反射第一定律 把接口转为反射对象vf
  vxs := reflect.ValueOf(args)
  ys := make([]interface{}, vxs.Len())

  for i := 0; i < vxs.Len(); i++ {
    ys[i] = vf.Call([]reflect.Value{vxs.Index(i)})[0].Interface() //反射第二定律 把反射类型转为接口
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


/*
检查类型是否具有某个方法 C++无法在运行期做到这个事情 因为C++像不托管语言c#或java那样具备反射功能 然而我们可以在编译期检查类型是否具有某个方法 
我们先定义一个检查方法是否存在的元函数（关于元函的概念数读者可以参考我在《程序员》2015年3月刊上的文章《C++11模版元编程》
c++没有反射必须要通过某种方法来获取对象的元信息
template<typename T> struct has_member_foo {
private:
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().foo(), std::true_type());
    template<typename U> static std::false_type Check(...);
public:
    enum{ value = std::is_same<decltype(Check<T>(0)), std::true_type>::value }; 
};
https://www.cnblogs.com/qicosmos/p/4772389.html 看不懂了
*/
