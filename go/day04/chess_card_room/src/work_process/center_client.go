package work_process

import (
    "errors"
    "encoding/json"
    "ipc"
)

type Center_client struct {
    *ipc.Ipc_client //center_client关联到ipc_client 目的是为了用ipc_client的call函数与server通信
}

func (client *Center_client)AddPlayer(player *Player) error {
    b, err := json.Marshal(*player)
    if err != nil {
        return err
    }
    resp, err := client.Call("addplayer", string(b))
    if err == nil && resp.Code == "200" {
        return nil
    }
    return err
}

func (client *Center_client)RemovePlayer(name string) error {
    ret, _ := client.Call("removeplayer", name)
    if ret.Code == "200" {
        return nil
    }
    return errors.New(ret.Code)
}

func (client *Center_client)ListPlayer(params string)(ps []*Player, err error) {
    resp, _ := client.Call("listplayer", params)
    if resp.Code != "200" {
        err = errors.New(resp.Code)
        return
    }
    err = json.Unmarshal([]byte(resp.Body), &ps)
    return
}

func (client *Center_client)Broadcast(message string) error {

    m := &Message{Content:message} // 构造Message结构体
    b, err := json.Marshal(m)
    if err != nil {
        return err
    }
    resp, _ := client.Call("broadcast", string(b))
    if resp.Code == "200" {
        return nil
    }
    return errors.New(resp.Code)
}

