package main
import (
	"fmt"
)

func (set *threadSafeSet) Iter() <-chan interface{} {
	ch := make(chan interface{})
	go func() {
		set.RLock()

		for elem := range set.s {
			ch <- elem
		}
		close(ch)

		set.RUnlock()

	}()
	return ch
}
