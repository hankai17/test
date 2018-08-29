1类型系统
	基础类型：int bool float byte
	复合类型：数组 结构体 指针
	指向任何对象的类型：any
	值语意和引用语意
	面向对象
	接口
	给类型添加方法：
	type Integer int
	func (a Integet) Less(b Integer) bool {
		return a < b
	}
	首先定义一个新类型Integer 然后添加方法
	fun main() {
	    var a Integer = 1
	    if a.Less(2) {
		fmt.Println(a,"Less 2")
	    }
	}
	若是做对比建议读一下深度探索c++对象模型
	c++面向对象是在c的基础上添加一个语法糖
	c++面向对象的核心就是：隐藏this指针:
	面向对象
	func (a Integet) Less(b Integer) bool {
		return a < b
	}
	面向过程
	func Integer_Less(a, b Integer) bool {
		return a < b
	}
	a.Less(2)
	Integer_Less(a,2)
	为什么隐藏了this(少一个参数)就是面向对象？ 因为this就是一个对象,
	面向对象就是面向this this有什么作用,为何如此之吊呢？ 因为this里
	封装了很多东西，这些东西组成了对象的很多功能......

	go的值和引用传递
	func (a *Integer) Add(b Integer) {
		*a += b
	}

	func (a Integer) Add(b Integer) {
		a += b
	}
	func main() {
		var a Integer = 1
		a.Add(2)
		fmt.Println("a=",a)
		    }
	记住go中大都是值语义(复制一份) 除了：
	数组切片：指向数组array的一个区间  []T
	map：键值查询
	channel：执行体(goroutine) 间的通信设施
	接口：对一组满足某个契约的类型的抽象
	结构体
	go的结构体只有 组合 特性
	type Rect struct {
	    x, y float64
	    width, height float64
	}
	func (r *Rect) Area() float64 {
		return r.width * r.height
	}

2初始化
	r1 := new(Rect)
	r2 := &new{}
	r3 := &Rect{0, 0, 100, 100}
	r4 := &Rect{width: 100, height: 100}
	未被初始化则为0
	一般而言：
	对象初始化由一个全局的创建函数完成 通常以newxxx命名：
	func NewRect(x, y, width, height float64) *Rect {
		return &Rect{x, y, width, height}
	}

3匿名组合
	组合就是继承
	type Base struct {
	    Name string
	}
	func (base *Base) Foo() {...}
	func (base *Base) Bar() {...}
	type Other struct {
	    Base
	    ....
	}
	func (other *Other) Bar {
	    other.Base.Bar()  //继承并改写
	}
	在调用时 other.Foo() 与 other.Base.Foo()一样
	因为没有改写基类的方法
	指针派生
	上面是普通派生 指针派生需要提供一个实例

4可见性
	小写本文件可见
	大写全可见

5接口
	go的核心

