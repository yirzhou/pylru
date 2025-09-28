#!/usr/bin/env python3
"""
Simple usage example of pylru

Install first with: pip install .
"""

import pylru


def main():
    # Create an LRU cache with capacity 3
    cache = pylru.LRU(3)
    print("Created LRU cache with capacity 3")
    
    # Add some key-value pairs (currently supports int->int)
    cache.put(1, 100)
    cache.put(2, 200) 
    cache.put(3, 300)
    print("Added three items")
    
    # Access some items (this affects their recency)
    result1 = cache.get(1)
    result2 = cache.get(2)
    print(f"cache.get(1): {result1} -> value: {result1.value()}")  # Should be FOUND, 100
    print(f"cache.get(2): {result2} -> value: {result2.value()}")  # Should be FOUND, 200
    
    # Add another item, which will evict the least recently used (3)
    cache.put(4, 400)
    print("Added item 4")
    
    # Check what's still in the cache
    result1_after = cache.get(1)
    result2_after = cache.get(2)
    result3_after = cache.get(3)  # Should be NOT_FOUND (evicted)
    result4_after = cache.get(4)
    
    print(f"cache.get(1): {result1_after}")
    if result1_after.is_found():
        print(f"  -> value: {result1_after.value()}")  # Should be 100
        
    print(f"cache.get(2): {result2_after}")
    if result2_after.is_found():
        print(f"  -> value: {result2_after.value()}")  # Should be 200
        
    print(f"cache.get(3): {result3_after}")  # Should show NOT_FOUND
    
    print(f"cache.get(4): {result4_after}")
    if result4_after.is_found():
        print(f"  -> value: {result4_after.value()}")  # Should be 400
    
    # Demonstrate safe value access with default
    print(f"\nUsing value_or() for safe access:")
    print(f"cache.get(3).value_or(-1): {cache.get(3).value_or(-1)}")  # -1 (default)
    print(f"cache.get(4).value_or(-1): {cache.get(4).value_or(-1)}")  # 400 (actual value)

if __name__ == "__main__":
    main()
