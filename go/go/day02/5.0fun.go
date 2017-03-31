1函数定义
	package  mymath
	import "errors"
	func Add(a int, b int) (ret int, err error) {
		if a < 0 || b < 0 {
		     err = errors.New("should be non-negative num")
		     return
		}
		return a + b, nil
	}
	若a b类型相同 可写为 func Add(a, b int) (ret int, err error) {
	若返回值只有一个 可写为 func Add(a, b int) int {

2函数调用
	只要导入包 就可以用这个包里的函数
	import "mymath"
	c := mymath.Add(1,2)
	记住 Add必须要大写 大写才对外可见

3不定参数
	

