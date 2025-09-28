#!/usr/bin/env python3
"""
Test script for pylru Python binding

This script tests the installed pylru package.
Make sure to install it first with: pip install .
"""
import sys

try:
    import pylru
except ImportError as e:
    print("❌ Failed to import pylru. Make sure to install it first with:")
    print("   pip install .")
    print(f"   Error: {e}")
    sys.exit(1)


def test_lru_cache():
    """Test the LRU cache functionality"""
    print("🧪 Testing Python LRU binding with Result-based API...")
    
    # Create LRU cache with capacity 2
    cache = pylru.LRU(2)
    print(f"✅ Created cache: {repr(cache)}")
    
    # Test basic put/get
    cache.put(1, 10)
    cache.put(2, 20)
    
    result1 = cache.get(1)
    result2 = cache.get(2)
    
    assert result1.is_found(), "Expected cache.get(1) to be found"
    assert result1.value() == 10, f"Expected result1.value() == 10, got {result1.value()}"
    assert result2.is_found(), "Expected cache.get(2) to be found"
    assert result2.value() == 20, f"Expected result2.value() == 20, got {result2.value()}"
    print("✅ Basic put/get works")
    
    # Test Result representation
    print(f"✅ Result repr: {repr(result1)}")
    
    # Test LRU eviction - accessing key 1 should make it more recent
    cache.get(1)  # Make key 1 most recent
    cache.put(3, 30)  # This should evict key 2
    
    result_evicted = cache.get(2)
    result_present1 = cache.get(1)
    result_present3 = cache.get(3)
    
    assert result_evicted.is_not_found(), "Expected cache.get(2) to be NOT_FOUND (evicted)"
    assert result_evicted.status() == pylru.Status.NOT_FOUND, "Expected status to be NOT_FOUND"
    assert result_present1.is_found(), "Expected cache.get(1) to be found (still present)"
    assert result_present1.value() == 10, f"Expected result_present1.value() == 10"
    assert result_present3.is_found(), "Expected cache.get(3) to be found"
    assert result_present3.value() == 30, f"Expected result_present3.value() == 30"
    print("✅ LRU eviction works correctly")
    
    # Test capacity constraint
    cache.put(4, 40)  # This should evict key 1
    
    result_evicted2 = cache.get(1)
    result_present_3 = cache.get(3)
    result_present_4 = cache.get(4)
    
    assert result_evicted2.is_not_found(), "Expected cache.get(1) to be NOT_FOUND (evicted)"
    assert result_present_3.is_found(), "Expected cache.get(3) to be found"
    assert result_present_3.value() == 30, f"Expected result_present_3.value() == 30"
    assert result_present_4.is_found(), "Expected cache.get(4) to be found"
    assert result_present_4.value() == 40, f"Expected result_present_4.value() == 40"
    print("✅ Capacity constraint enforced")
    
    # Test updating existing key
    cache.put(3, 33)  # Update key 3
    result_updated = cache.get(3)
    result_unchanged = cache.get(4)
    
    assert result_updated.is_found(), "Expected cache.get(3) to be found"
    assert result_updated.value() == 33, f"Expected result_updated.value() == 33 (updated)"
    assert result_unchanged.is_found(), "Expected cache.get(4) to be found"
    assert result_unchanged.value() == 40, f"Expected result_unchanged.value() == 40 (unchanged)"
    print("✅ Key update works")
    
    # Test Result bool conversion and value_or
    not_found_result = cache.get(999)  # Key that doesn't exist
    assert not not_found_result, "Expected not_found_result to be falsy"
    assert not_found_result.value_or(-1) == -1, "Expected value_or(-1) to return -1"
    
    found_result = cache.get(3)
    assert found_result, "Expected found_result to be truthy"
    assert found_result.value_or(-1) == 33, "Expected value_or(-1) to return actual value"
    print("✅ Result bool conversion and value_or work")
    
    print("🎉 All tests passed!")

if __name__ == "__main__":
    test_lru_cache()

