package main

import "fmt"

func main() {
    loop:
        for {
            switch {
            case true:
                fmt.Println("breaking out...")
                //break
                break loop
            }
        }

    fmt.Println("out!")
}
