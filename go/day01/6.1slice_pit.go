package main

import "fmt"

func main1() {
    slice := []int{0, 1, 2, 3}
    myMap := make(map[int]*int)

    for index, value := range slice {
        fmt.Printf("value=%v\n",value)
        fmt.Printf("&value=%v\n",&value) //即range返回的是一个重复的地址
        a := value
        myMap[index] = &a
        fmt.Println(&a)
    }
    fmt.Println("=====new map=====")
    prtMap(myMap)
}

func prtMap(myMap map[int]*int) {
    for key, value := range myMap {
        fmt.Printf("map[%v]=%v\n", key, *value)
    }
}

type student struct {
	Name string
	Age  int
}

func pase_student() {
	m := make(map[string]*student)
	//m := make(map[string]student)

	stus := []student {
		{Name: "zhou", Age: 24},
		{Name: "li", Age: 23},
		{Name: "wang", Age: 22},
	}

	for _, stu := range stus {
		m[stu.Name] = &stu
		//m[stu.Name] = stu
		fmt.Printf("%p\n", &stu)
	}

	for k, v := range m {
		fmt.Println("k=",k," v=",v)
    }
}

func main() {
	pase_student()
	fmt.Printf("\n")
}
//会重用地址也就是说for _, stu := range stus 中的 stu 总是在同一个地址

    /*
    stus := []*student {
      &student{Name: "zhou", Age: 24},
      &student{Name: "li", Age: 23},
      &student{Name: "wang", Age: 22},
    }
    */
