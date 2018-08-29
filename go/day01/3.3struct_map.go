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

func main() {
	//var ua = UserAges {
	//var ua =  &UserAges {
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
