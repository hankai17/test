package main

import (
	"fmt"
)

type People interface {
	Speak(string) string
}

type Stduent struct{}
func (stu *Stduent) Speak(think string) (talk string) {
	if think == "bad" {
		talk = "You are a good boy"
	} else {
		talk = "hi"
	}
	return
}

func main() {
	//var peo People = Stduent{}
	var peo People = &Stduent{}
	fmt.Println(peo.Speak("bad"))
}
