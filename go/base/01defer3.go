package main

func main() {
	println(DeferFunc1(1))
	println(DeferFunc2(1))
	println(DeferFunc3(1))
}

func DeferFunc1(i int) (t int) { //4 先defer后return
	t = i
	defer func() {
		t += 3
	}()
	return t
}

func DeferFunc2(i int) int { //1
	t := i
	defer func() {
		t += 3
	}()
	return t
}

func DeferFunc3(i int) (t int) { //3
	defer func() {
		t += i
	}()
	return 2
}
