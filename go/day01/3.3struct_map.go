package main
import (
	"fmt"
	"sync"
)

type UserAges struct {
	ages map[string]int
	sync.Mutex
}
func (ua *UserAges) Add(name string, age int) {
	ua.Lock()
	defer ua.Unlock()
	ua.ages[name] = age
}
func (ua *UserAges) Get(name string) int {
	ua.Lock()
	defer ua.Unlock()
	if age, ok := ua.ages[name]; ok {
		return age
	}
	return -1
}

func main1() {
	//var ua = UserAges {
	var ua =  &UserAges {
		ages:make(map[string]int),
	}
	//var s *obj = new(class)
	ua.Add("mike", 18);
	ua.Add("dog", 18);
	fmt.Println("--")
}

/*
intmap := map[int]string {
        1:"a",
        2:"bb",
        3:"ccc",
    }

*/

////////////////////////////////////////// 很明显这样写复杂了  因为是个结构体包装的mutex mutex可以直接写出匿名的方式 这样新的结构体算是继承了mutex可以直接加锁
type MyMap struct {
	m map[string] string
	mtx sync.Mutex
}
func (mm *MyMap) Add(key string, val string) {
	mm.mtx.Lock()
	defer mm.mtx.Unlock()
	mm.m[key] = val
}
func (mm *MyMap) Get(key string) string {
	mm.mtx.Lock()
	defer mm.mtx.Unlock()
	if val, ok := mm.m[key]; ok {
		return val
	}
	return "x"
}

func main() {
	var test = MyMap {
		m:make(map[string]string),
	}
	test.Add("1", "1")
	test.Add("2", "2")
	fmt.Println(test.Get("2"))
}
