package main

import (
	"container/heap"
	"fmt"
)

//
// Cache Class
//
type Cache struct {
	store  map[string]*CacheEntry
	pqueue PQueue

	capacity uint64
	free     uint64

	gen int64 // generation counter
}
type CacheEntry struct {
	value   string
	key     string
	prio    int64
	pqIndex int
}
type PQueue []*CacheEntry

func mkcache(capacity uint64) *Cache {
	c := new(Cache)
	c.capacity = capacity
	c.free = capacity
	c.gen = 0
	c.store = make(map[string](*CacheEntry))
	c.pqueue = make(PQueue, 0, capacity)
	heap.Init(&c.pqueue)
	return c
}

func (c *Cache) set(k string, v string) error {
	fmt.Printf("[C Set] %s=%s\n", k, v)
	c.gen += 1
	_, ok := c.store[k]
	if ok {
		c.store[k].key = k
		c.store[k].prio = -c.gen
		heap.Fix(&c.pqueue, c.store[k].pqIndex)
	} else {
		if c.free <= 0 {
			c.evict()
		}
		c.free -= 1
		item := &CacheEntry{
			key:   k,
			value: v,
			prio:  -c.gen,
		}
		c.store[k] = item
		heap.Push(&c.pqueue, item)
	}
	return nil
}

func (c *Cache) evict() {
	item := heap.Pop(&c.pqueue).(*CacheEntry)
	delete(c.store, item.key)
	fmt.Printf("[C Evict] %s, prio=%d\n", item.key, item.prio)
	c.free += 1
}
func (c *Cache) get(k string) (v string, found bool) {
	fmt.Printf("[C Get] %s\n", k)
	c.gen += 1
	item, ok := c.store[k]
	if !ok {
		return "", ok
	}
	item.prio = -c.gen
	heap.Fix(&c.pqueue, item.pqIndex)
	return item.key, ok
}

//
// Priority Queue Class
//
func (pqs *PQueue) dump(msg string) { // debugging helper
	pq := *pqs
	fmt.Printf("[%s] len=%d cap=%d pqs=%p %+v\n", msg, len(pq), cap(pq), pqs, pq)
}

// In order to use the heap.* functions, PQueue needs to implement
// the heap.interface type:
func (pq PQueue) Len() int           { return len(pq) }
func (pq PQueue) Less(i, j int) bool { return pq[i].prio > pq[j].prio }
func (pq PQueue) Swap(i, j int)      { pq[i], pq[j] = pq[j], pq[i] }
func (pqs *PQueue) Push(x interface{}) {
	pqs.dump("PQ Push")
	pq := *pqs
	if len(pq) == cap(pq) {
		panic("Can't push a full queue")
	}
	pq = pq[:len(pq)+1] // extend slice
	pq[len(pq)-1] = x.(*CacheEntry)
	*pqs = pq
}
func (pqs *PQueue) Pop() interface{} {
	pqs.dump("PQ Pop")
	pq := *pqs
	item := pq[len(pq)-1]
	pq[len(pq)-1] = nil
	*pqs = pq[:len(pq)-1]
	return item
}

func main() {
	c := mkcache(3)
	c.set("k1", "v1")
	c.set("k2", "v2")
	c.set("k3", "v3")
	c.set("k4", "v4") // should evict k1
	c.get("k2")
	c.set("k5", "v5") // should evict k3
}
