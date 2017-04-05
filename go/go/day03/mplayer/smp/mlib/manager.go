package mlib

import "errors"

type MusicManager struct { //为什么封装成一个结构体？  因为得把它放到一个包里面 所以封装成一个结构体合适不过
	musics []MusicEntry  //这是切片还是数组？ //明显是个切片 因为数组是一个具体的值
}

func NewMusicManager() *MusicManager {
	return &MusicManager{make([]MusicEntry, 0)}  //结构体初始化 + slice初始化
}

func (m *MusicManager) Len() int {
	return len(m.musics)
}

func (m *MusicManager) Get(index int) (music *MusicEntry, err error) {
	if index < 0 || index >= len(m.musics) {
		return nil, errors.New("Index out of range.")
	}
	return &m.musics[index], nil
}

func (m *MusicManager) Find(name string) *MusicEntry { //轮询for的方式查找
	if len(m.musics) == 0 {
		return nil
	}

	for _, m := range m.musics {
		if m.Name == name {
			return &m
		}
	}
	return nil
}

func (m *MusicManager) Add(music *MusicEntry) {  //切片的好处 

	m.musics = append(m.musics, *music)
}

func (m *MusicManager) Remove(index int) *MusicEntry {
	if index < 0 || index >= len(m.musics) {
		return nil
	}

	removedMusic := &m.musics[index]
	m.musics = append(m.musics[:index], m.musics[index+1:]...) //切片 + 切片 别忘了...
	return removedMusic
}

func (m *MusicManager) RemoveByName(name string) *MusicEntry {  
    if len(m.musics) == 0 {
        return nil
    }

    for i, v := range m.musics {
        if v.Name == name {
            return m.Remove(i) //调用上main的Remove函数
        }
    }
    return nil
}

