#!/usr/bin/env python3
"""
Python Performance Test for LRU Cache Implementations

Compares:
1. pylru.LRU (our C++ implementation with Python bindings)
2. OrderedDict-based LRU (Python implementation)
3. Regular dict (baseline - no LRU behavior)
"""

import os
import random
import statistics
import sys
import time
from collections import OrderedDict
from typing import Any, Dict, List, Optional, Tuple

# Add the build directory to path to import pylru
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'build', 'src'))

try:
    import pylru
    PYLRU_AVAILABLE = True
except ImportError:
    PYLRU_AVAILABLE = False
    print("Warning: pylru not available. Install with 'pip install .' first")


class OrderedDictLRU:
    """LRU Cache implementation using OrderedDict for comparison"""
    
    def __init__(self, capacity: int):
        self.capacity = capacity
        self.cache: OrderedDict = OrderedDict()
    
    def get(self, key: int) -> Optional[int]:
        if key in self.cache:
            # Move to end (most recently used)
            value = self.cache.pop(key)
            self.cache[key] = value
            return value
        return None
    
    def put(self, key: int, value: int) -> None:
        if key in self.cache:
            # Update existing key
            self.cache.pop(key)
        elif len(self.cache) >= self.capacity:
            # Remove least recently used (first item)
            self.cache.popitem(last=False)
        
        self.cache[key] = value


class DictCache:
    """Simple dict-based cache (no LRU behavior) for baseline comparison"""
    
    def __init__(self, capacity: int):
        self.capacity = capacity
        self.cache: Dict[int, int] = {}
    
    def get(self, key: int) -> Optional[int]:
        return self.cache.get(key)
    
    def put(self, key: int, value: int) -> None:
        if len(self.cache) >= self.capacity and key not in self.cache:
            # Simple eviction: remove a random key
            if self.cache:
                random_key = random.choice(list(self.cache.keys()))
                del self.cache[random_key]
        
        self.cache[key] = value


class PerformanceTester:
    
    def __init__(self):
        self.random = random.Random(42)  # Fixed seed for reproducible results
    
    def benchmark_implementation(self, cache_impl, name: str, total_ops: int, 
                                key_range: int, write_ratio: float) -> Dict[str, Any]:
        """Benchmark a cache implementation"""
        
        # Pre-populate some data
        for i in range(min(100, key_range // 10)):
            cache_impl.put(i, i * 10)
        
        hits = 0
        operations = 0
        
        start_time = time.perf_counter()
        
        for i in range(total_ops):
            key = self.random.randint(1, key_range)
            
            if self.random.random() < write_ratio:
                # Write operation
                cache_impl.put(key, key * 10)
            else:
                # Read operation  
                result = cache_impl.get(key)
                # Handle different return types
                if hasattr(result, 'is_found'):  # pylru Result object
                    if result.is_found():
                        hits += 1
                elif result is not None:  # OrderedDict/dict
                    hits += 1
            
            operations += 1
        
        end_time = time.perf_counter()
        duration = end_time - start_time
        
        # Calculate hit rate based on the actual operations performed
        read_ops = total_ops * (1 - write_ratio)
        if hasattr(cache_impl, '__class__') and 'pylru' in str(type(cache_impl)):
            # For pylru, count hits from Result objects (already counted during benchmark)
            hit_rate = hits / read_ops if read_ops > 0 else 0
        else:
            hit_rate = hits / read_ops if read_ops > 0 else 0
        
        qps = total_ops / duration
        avg_latency_us = (duration / total_ops) * 1e6  # microseconds
        
        return {
            'name': name,
            'qps': qps,
            'avg_latency_us': avg_latency_us,
            'total_ops': total_ops,
            'hit_rate': hit_rate,
            'duration': duration
        }
    
    def run_comparison_test(self, cache_size: int, total_ops: int, key_range: int, 
                           write_ratio: float, test_name: str) -> List[Dict[str, Any]]:
        """Run comparison test across all implementations"""
        
        results = []
        
        # Test pylru if available
        if PYLRU_AVAILABLE:
            cache = pylru.LRU(cache_size)
            result = self.benchmark_implementation(cache, "pylru (C++)", total_ops, 
                                                 key_range, write_ratio)
            results.append(result)
        
        # Test OrderedDict LRU
        cache = OrderedDictLRU(cache_size)
        result = self.benchmark_implementation(cache, "OrderedDict LRU", total_ops, 
                                             key_range, write_ratio)
        results.append(result)
        
        # Test simple dict
        cache = DictCache(cache_size)
        result = self.benchmark_implementation(cache, "dict (baseline)", total_ops, 
                                             key_range, write_ratio)
        results.append(result)
        
        return results
    
    def print_results(self, results: List[Dict[str, Any]], test_name: str):
        """Print benchmark results in a formatted table"""
        
        print(f"\n{'='*80}")
        print(f"Python LRU Cache Comparison: {test_name}")
        print('='*80)
        print(f"{'Implementation':<20} {'QPS':<15} {'Latency (μs)':<15} {'Hit Rate':<12} {'Relative':<10}")
        print('-'*80)
        
        # Sort by QPS for easier comparison
        sorted_results = sorted(results, key=lambda x: x['qps'], reverse=True)
        best_qps = sorted_results[0]['qps'] if sorted_results else 1
        
        for result in sorted_results:
            relative_perf = result['qps'] / best_qps
            print(f"{result['name']:<20} "
                  f"{result['qps']:<15,.0f} "
                  f"{result['avg_latency_us']:<15.1f} "
                  f"{result['hit_rate']*100:<12.1f}% "
                  f"{relative_perf:<10.2f}x")
        
        print('='*80)
    
    def run_scaling_test(self):
        """Test performance scaling with different cache sizes"""
        
        print(f"\n{'='*60}")
        print("Cache Size Scaling Comparison")
        print('='*60)
        print(f"{'Size':<10} {'pylru QPS':<15} {'OrderedDict QPS':<15} {'dict QPS':<15} {'Speedup':<10}")
        print('-'*60)
        
        for size in [100, 500, 1000, 5000]:
            results = self.run_comparison_test(size, 50000, 10000, 0.3, f"Size {size}")
            
            # Extract QPS for each implementation
            qps_data = {r['name']: r['qps'] for r in results}
            
            pylru_qps = qps_data.get('pylru (C++)', 0)
            ordered_qps = qps_data.get('OrderedDict LRU', 0)
            dict_qps = qps_data.get('dict (baseline)', 0)
            
            speedup = pylru_qps / ordered_qps if ordered_qps > 0 else 0
            
            print(f"{size:<10} {pylru_qps:<15,.0f} {ordered_qps:<15,.0f} "
                  f"{dict_qps:<15,.0f} {speedup:<10.1f}x")
        
        print('-'*60)


def main():
    if not PYLRU_AVAILABLE:
        print("Error: pylru module not found!")
        print("Please install it first with: pip install .")
        sys.exit(1)
    
    print("Python LRU Cache Performance Comparison")
    print("=" * 50)
    
    tester = PerformanceTester()
    
    # Test configuration
    cache_size = 1000
    total_ops = 100000  # Smaller than C++ test due to Python overhead
    key_range = 10000
    
    print(f"Configuration:")
    print(f"  Cache Size: {cache_size}")
    print(f"  Total Operations: {total_ops:,}")
    print(f"  Key Range: {key_range:,}")
    print(f"  Language: Python {sys.version.split()[0]}")
    
    # Test 1: Random Access (30% writes, 70% reads)
    print("\n[1/4] Random Access Pattern...")
    results = tester.run_comparison_test(cache_size, total_ops, key_range, 0.3, 
                                        "Random Access")
    tester.print_results(results, "Random Access (30% writes, 70% reads)")
    
    # Test 2: Read-Heavy (10% writes, 90% reads)
    print("\n[2/4] Read-Heavy Workload...")
    results = tester.run_comparison_test(cache_size, total_ops, key_range, 0.1, 
                                        "Read-Heavy")
    tester.print_results(results, "Read-Heavy (10% writes, 90% reads)")
    
    # Test 3: Write-Heavy (70% writes, 30% reads)
    print("\n[3/4] Write-Heavy Workload...")
    results = tester.run_comparison_test(cache_size, total_ops, key_range, 0.7, 
                                        "Write-Heavy")
    tester.print_results(results, "Write-Heavy (70% writes, 30% reads)")
    
    # Test 4: Cache Size Scaling
    print("\n[4/4] Cache Size Scaling...")
    tester.run_scaling_test()
    
    # Summary
    print(f"\n{'='*80}")
    print("Summary")
    print('='*80)
    print("Key Insights:")
    print("• pylru (C++): Consistent performance with proper LRU semantics")
    print("• OrderedDict: Pure Python LRU, good semantics but slower")
    print("• dict: Fastest but no LRU behavior (unfair comparison)")
    print("\nRecommendation:")
    print("• Use pylru for performance-critical applications requiring LRU")
    print("• Use OrderedDict for small caches where pure Python is preferred")
    print("• Performance gap shows the value of C++ implementation for hot paths")
    print('='*80)


if __name__ == "__main__":
    main()
