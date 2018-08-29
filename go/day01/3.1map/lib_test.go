package lib

import (
  "testing"
)

func BenchmarkBuiltinMapStoreParalell(b *testing.B) {
    b.RunParallel(func(pb *testing.PB) {
        r := rand.New(rand.NewSource(time.Now().Unix()))
        for pb.Next() {
            // The loop body is executed b.N times total across all goroutines.
            k := r.Intn(100000000)
            builtinMapStore(k, k)
        }
    })
}

func BenchmarkSyncMapStoreParalell(b *testing.B) {
    b.RunParallel(func(pb *testing.PB) {
        r := rand.New(rand.NewSource(time.Now().Unix()))
        for pb.Next() {
            // The loop body is executed b.N times total across all goroutines.
            k := r.Intn(100000000)
            syncMapStore(k, k)
        }
    })
}
