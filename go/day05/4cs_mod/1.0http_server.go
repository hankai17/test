package main

import (
	"log"
	"net/http"
)

func main() {
	mux := http.NewServeMux()

	rh := http.RedirectHandler("http://www.baidu.com", 307)
	mux.Handle("/foo", rh)

	log.Println("Listening...")
	http.ListenAndServe(":3000", mux)
}
