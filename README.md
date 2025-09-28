# pylru

A high-performance LRU (Least Recently Used) cache implementation in C++ with Python bindings.

## Features

- 🚀 **High Performance**: 3.8M+ QPS single-threaded, sub-microsecond latency
- ⚡ **O(1) Complexity**: Guaranteed constant-time get/put operations
- 🐍 **Python Bindings**: Seamless Python integration using pybind11
- 🔧 **Template Design**: Flexible C++ templates for any key/value types
- ✅ **Type-safe Result API**: No more -1 for "not found" - proper status handling
- 🛡️ **Clear Status Handling**: `Status.FOUND` / `Status.NOT_FOUND` enum
- 📊 **Comprehensive Benchmarks**: Performance tests for different workload patterns
- ✅ **Thoroughly Tested**: Unit tests and performance validation
- 📦 **Multiple Build Systems**: CMake for development, pip for easy installation

## Installation

### Quick Start (Recommended)

```bash
# Install directly with pip (builds automatically)
pip install .

# Test the installation
python -c "import pylru; cache = pylru.LRU(2); cache.put(1, 42); result = cache.get(1); print(f'Found: {result.value()}' if result else 'Not found')"
```

### Development Build (CMake)

For C++ development or if you want to use CMake:

```bash
mkdir build && cd build
cmake ..
make

# Run C++ tests
./tests/lru.t

# Run C++ performance benchmarks
./tests/lru.perf

# Run Python performance comparison (from project root)
cd .. && python3 tests/python_performance_test.py
```

### Prerequisites

- **For pip install**: Python 3.7+ and a C++17 compiler
- **For CMake build**: CMake 3.4+, C++17 compiler, pybind11

## Usage

### C++ Usage

```cpp
#include <lru.h>

pylru::lru<int, int> cache(2);  // Capacity of 2
cache.put(1, 10);
cache.put(2, 20);
int value = cache.get(1);  // Returns 10
```

### Python Usage

```python
import pylru

cache = pylru.LRU(2)  # Capacity of 2
cache.put(1, 10)      # Store key=1, value=10
cache.put(2, 20)      # Store key=2, value=20

# Get returns a Result object with status information
result = cache.get(1)
if result.is_found():
    print(f"Found: {result.value()}")  # Found: 10

# LRU eviction in action
cache.put(3, 30)      # Evicts least recently used (key 2)

# Check evicted item
evicted = cache.get(2)
print(evicted.is_not_found())        # True
print(evicted.status())              # Status.NOT_FOUND
print(evicted.value_or(-1))          # -1 (default value)

# Check existing items
print(cache.get(1).value())          # 10
print(cache.get(3).value())          # 30

# Convenient boolean conversion
if cache.get(1):  # True if found
    print("Key 1 exists!")
```

## Architecture

- `include/node.h` - Doubly-linked list node
- `include/linked_list.h` - Doubly-linked list implementation
- `include/result.h` - Result type with status information
- `include/lru.h` - Main LRU cache template class
- `src/pylru.h` - Python bindings using pybind11
- `tests/lru.t.cpp` - C++ unit tests

### Result API

The `get()` method returns a `Result<T>` object instead of raw values:

**Python API:**

- `result.is_found()` / `result.is_not_found()` - Check status
- `result.status()` - Get `Status.FOUND` or `Status.NOT_FOUND`
- `result.value()` - Get value (throws if not found)
- `result.value_or(default)` - Safe access with fallback
- `bool(result)` - Convert to boolean (`True` if found)

**C++ API:**

```cpp
auto result = cache.get(key);
if (result.is_found()) {
    auto value = result.value();
} else {
    // Handle not found case
    auto default_val = result.value_or(-1);
}
```

## Performance

The LRU cache delivers excellent performance:

- **3.8M+ QPS** for random access patterns (single-threaded)
- **15.2M QPS** for sequential access patterns
- **Sub-microsecond latency**: 66-371ns per operation
- **Efficient scaling**: Performance stable across cache sizes

See [PERFORMANCE.md](PERFORMANCE.md) for detailed C++ benchmarks and [PYTHON_PERFORMANCE.md](PYTHON_PERFORMANCE.md) for Python binding performance analysis.

## Build Systems

This project uses CMake for development and pip for distribution. For larger enterprise projects, consider migrating to Bazel.
