# Caching

Task: 

Implement a cache with the following interface:

- Get: returns the cached value with the given key if available.
- Set: store the value with the given key.

An LRU cache has a given capacity and evicts the least recently used entry once full. 

Example:

```
c = LRUCache(3)

c.set(1, "a")
c.set(2, "b")
c.set(3, "c")

c.get(2) # should print "b"
c.get(1) # should print "a"

c.set(4, "d") # evicts (3, "c") as (1, "a") and (2, "b") were accessed more recently.

c.get(3) # should return None
```
