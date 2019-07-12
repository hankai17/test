package main

import (
    "fmt"
    "os"
    "strings"
    "time"
    "strconv"
    "math/rand"
    "net/http"
    "io/ioutil"
)

const (
    FILESIZE = 150000000
)

func get_base_rand_range() (rn string){
HEAD:
    min := rand.Intn(FILESIZE)
    max := rand.Intn(FILESIZE)
    if min != max {
        if min > max {
            tmp := min
            min = max
            max = tmp
        }
        return strconv.Itoa(min) + "-" + strconv.Itoa(max)
    } else {
        goto HEAD
    }
}

func req_ppc(ch chan string, i int, rn string) {
    client := &http.Client{}
    url := ""
    if i == 0 {
        url = "http://10.0.120.35:80/2Q2WFE0F41965EE9590D1AC46148A801F940902F6293_0_4322428091AA5C9A321C067BE1D47642E8097736_0/soft.duote.com.cn/qq_9.1.0.24712.exe"
    } else {
        url = "http://10.0.120.106:80/qq_9.1.0.24712.exe"
    }

    reqest, err := http.NewRequest("GET", url, nil)
    reqest.Header.Add("Range", "bytes=" + rn)

    if err != nil {
        fmt.Println(err)
        return
    }

    resp, err := client.Do(reqest)
    if resp != nil {
        defer resp.Body.Close()
    }

    if err != nil {
        fmt.Println(err)
        return
    }

    body, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        fmt.Println(err)
        return
    }
    //fmt.Println(string(body))
    ch <- string(body)
}

func comp_task(ch chan int, j int) {
    _ = j
    chs := make([]chan string, 2)
    rn := get_base_rand_range()
    fmt.Println(rn)
    for i := 0; i < 2; i++ {
        chs[i] = make(chan(string))
        go req_ppc(chs[i], i, rn)
    }

    defer func() {
        close(chs[0])
        close(chs[1])
    }()

    timeout := time.After(time.Second * 50)
    data1 := ""
    data2 := ""

    for {
        select {
                        case data1 = <- chs[0]:
                            //fmt.Println(data1)
                        case data2 = <- chs[1]:
                            //fmt.Println(data2)
                        case <- timeout:
                                //fmt.Println("3s has time out")
                                goto FOREND
                }
        }
        FOREND:

        if data1 == "" || data2 == "" {
                fmt.Println("result is nil!")
                return
        }
        if data1 != "" && data2 != "" {
               //fmt.Println(j, " result: ", strings.Compare(data1, data2)) 
               //fmt.Println("result: ", strings.Compare("1", "1")) 
               //fmt.Println("result: ", strings.Compare("0", "1")) 
               f, err := os.Create(rn + "_1.exe")
               if err != nil {
                    fmt.Println("create file ", rn + "_1.exe err")
               }
               defer f.Close()
               f.WriteString(data1)

               f1, err := os.Create(rn + "_2.exe")
               if err != nil {
                    fmt.Println("create file ", rn + "_2.exe err")
               }
               defer f1.Close()
               f1.WriteString(data2)

               ch <- strings.Compare(data1, data2)
        }
}

func main() {
    chs := make(chan int)
    for i := 0; i < 20; i++ {
        go comp_task(chs, i)
    }
    defer func() {
        close(chs)
    }()

    timeout := time.After(time.Second * 60)
    data1 := -2

    for {
        select {
                case data1 = <- chs:
                    fmt.Println(data1)
                case <- timeout:
                    fmt.Println("60s timeout")
                    goto FOREND
        } 
    }
    FOREND:
    return
}
