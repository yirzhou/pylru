#include <chrono>
#include <iomanip>
#include <iostream>
#include <lru.h>
#include <random>
#include <vector>

using namespace pylru;
using namespace std::chrono;

class PerformanceTester {
  private:
    std::mt19937 rng{std::random_device{}()};

  public:
    struct BenchmarkResult {
        std::string test_name;
        double ops_per_second;
        double avg_latency_ns;
        size_t total_ops;
        double hit_rate;
    };

    // Test random access pattern
    BenchmarkResult test_random_access(int cache_size, int total_ops,
                                       int key_range) {
        lru<int, int> cache(cache_size);
        std::uniform_int_distribution<int> dist(1, key_range);

        // Pre-populate some data
        for (int i = 1; i <= cache_size / 2; ++i) { cache.put(i, i * 10); }

        size_t hits = 0;
        auto start = high_resolution_clock::now();

        for (int i = 0; i < total_ops; ++i) {
            int key = dist(rng);
            if (i % 3 == 0) { // 33% writes, 67% reads
                cache.put(key, key * 10);
            } else {
                auto result = cache.get(key);
                if (result.is_found()) { hits++; }
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);

        return {
            "Random Access (33% writes, 67% reads)",
            static_cast<double>(total_ops) / (duration.count() / 1e9),
            static_cast<double>(duration.count()) / total_ops,
            static_cast<size_t>(total_ops),
            static_cast<double>(hits) /
                (total_ops * 2 / 3) // Only count read operations
        };
    }

    // Test sequential access pattern
    BenchmarkResult test_sequential_access(int cache_size, int total_ops) {
        lru<int, int> cache(cache_size);

        size_t hits = 0;
        auto start = high_resolution_clock::now();

        for (int i = 0; i < total_ops; ++i) {
            int key = i % (cache_size * 2); // Cycle through 2x cache size
            if (i % 4 == 0) {               // 25% writes, 75% reads
                cache.put(key, key * 10);
            } else {
                auto result = cache.get(key);
                if (result.is_found()) { hits++; }
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);

        return {"Sequential Access (25% writes, 75% reads)",
                static_cast<double>(total_ops) / (duration.count() / 1e9),
                static_cast<double>(duration.count()) / total_ops,
                static_cast<size_t>(total_ops),
                static_cast<double>(hits) / (total_ops * 3 / 4)};
    }

    // Test hot key pattern (80/20 rule)
    BenchmarkResult test_hot_key_pattern(int cache_size, int total_ops,
                                         int key_range) {
        lru<int, int> cache(cache_size);
        std::uniform_int_distribution<int> hot_dist(1, key_range /
                                                           5); // 20% of keys
        std::uniform_int_distribution<int> cold_dist(key_range / 5 + 1,
                                                     key_range); // 80% of keys
        std::uniform_real_distribution<double> selector(0.0, 1.0);

        size_t hits = 0;
        auto start = high_resolution_clock::now();

        for (int i = 0; i < total_ops; ++i) {
            // 80% of accesses go to 20% of keys (hot keys)
            int key = (selector(rng) < 0.8) ? hot_dist(rng) : cold_dist(rng);

            if (i % 5 == 0) { // 20% writes, 80% reads
                cache.put(key, key * 10);
            } else {
                auto result = cache.get(key);
                if (result.is_found()) { hits++; }
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);

        return {"Hot Key Pattern (20% writes, 80% reads, 80/20 rule)",
                static_cast<double>(total_ops) / (duration.count() / 1e9),
                static_cast<double>(duration.count()) / total_ops,
                static_cast<size_t>(total_ops),
                static_cast<double>(hits) / (total_ops * 4 / 5)};
    }

    // Test read-heavy workload
    BenchmarkResult test_read_heavy(int cache_size, int total_ops,
                                    int key_range) {
        lru<int, int> cache(cache_size);
        std::uniform_int_distribution<int> dist(1, key_range);

        // Pre-populate cache to 80% capacity
        for (int i = 1; i <= cache_size * 0.8; ++i) { cache.put(i, i * 10); }

        size_t hits = 0;
        auto start = high_resolution_clock::now();

        for (int i = 0; i < total_ops; ++i) {
            int key = dist(rng);
            if (i % 10 == 0) { // 10% writes, 90% reads
                cache.put(key, key * 10);
            } else {
                auto result = cache.get(key);
                if (result.is_found()) { hits++; }
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);

        return {"Read-Heavy (10% writes, 90% reads)",
                static_cast<double>(total_ops) / (duration.count() / 1e9),
                static_cast<double>(duration.count()) / total_ops,
                static_cast<size_t>(total_ops),
                static_cast<double>(hits) / (total_ops * 9 / 10)};
    }

    // Test write-heavy workload
    BenchmarkResult test_write_heavy(int cache_size, int total_ops,
                                     int key_range) {
        lru<int, int> cache(cache_size);
        std::uniform_int_distribution<int> dist(1, key_range);

        size_t hits = 0;
        auto start = high_resolution_clock::now();

        for (int i = 0; i < total_ops; ++i) {
            int key = dist(rng);
            if (i % 3 != 0) { // 67% writes, 33% reads
                cache.put(key, key * 10);
            } else {
                auto result = cache.get(key);
                if (result.is_found()) { hits++; }
            }
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);

        return {"Write-Heavy (67% writes, 33% reads)",
                static_cast<double>(total_ops) / (duration.count() / 1e9),
                static_cast<double>(duration.count()) / total_ops,
                static_cast<size_t>(total_ops),
                static_cast<double>(hits) / (total_ops / 3)};
    }

    void print_results(const std::vector<BenchmarkResult> &results) {
        std::cout << "\n" << std::string(100, '=') << "\n";
        std::cout << "LRU Cache Performance Benchmark Results\n";
        std::cout << std::string(100, '=') << "\n";

        std::cout << std::left << std::setw(45) << "Test Name" << std::setw(15)
                  << "QPS" << std::setw(15) << "Avg Latency" << std::setw(12)
                  << "Total Ops" << std::setw(10) << "Hit Rate" << "\n";
        std::cout << std::string(100, '-') << "\n";

        for (const auto &result : results) {
            std::cout << std::left << std::setw(45) << result.test_name
                      << std::setw(15) << std::fixed << std::setprecision(0)
                      << result.ops_per_second << std::setw(15) << std::fixed
                      << std::setprecision(1) << (result.avg_latency_ns) << "ns"
                      << std::setw(12) << result.total_ops << std::setw(10)
                      << std::fixed << std::setprecision(1)
                      << (result.hit_rate * 100) << "%" << "\n";
        }
        std::cout << std::string(100, '=') << "\n";
    }
};

int main() {
    std::cout << "Starting LRU Cache Performance Tests...\n";

    PerformanceTester tester;
    std::vector<PerformanceTester::BenchmarkResult> results;

    const int cache_size = 1000;
    const int total_ops = 1000000; // 1M operations
    const int key_range = 10000;   // 10K possible keys

    std::cout << "\nTest Configuration:\n";
    std::cout << "  Cache Size: " << cache_size << "\n";
    std::cout << "  Total Operations: " << total_ops << "\n";
    std::cout << "  Key Range: " << key_range << "\n";
    std::cout << "  Hardware: Single-threaded\n";

    std::cout << "\nRunning benchmarks...\n";

    // Run different access pattern tests
    std::cout << "[1/5] Random Access Pattern... " << std::flush;
    results.push_back(
        tester.test_random_access(cache_size, total_ops, key_range));
    std::cout << "Done\n";

    std::cout << "[2/5] Sequential Access Pattern... " << std::flush;
    results.push_back(tester.test_sequential_access(cache_size, total_ops));
    std::cout << "Done\n";

    std::cout << "[3/5] Hot Key Pattern (80/20 rule)... " << std::flush;
    results.push_back(
        tester.test_hot_key_pattern(cache_size, total_ops, key_range));
    std::cout << "Done\n";

    std::cout << "[4/5] Read-Heavy Workload... " << std::flush;
    results.push_back(tester.test_read_heavy(cache_size, total_ops, key_range));
    std::cout << "Done\n";

    std::cout << "[5/5] Write-Heavy Workload... " << std::flush;
    results.push_back(
        tester.test_write_heavy(cache_size, total_ops, key_range));
    std::cout << "Done\n";

    // Print all results
    tester.print_results(results);

    // Test different cache sizes
    std::cout << "\nCache Size Scaling Test:\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left << std::setw(15) << "Cache Size" << std::setw(20)
              << "QPS (Random Access)" << std::setw(15) << "Hit Rate" << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (int size : {100, 500, 1000, 5000, 10000}) {
        auto result = tester.test_random_access(
            size, 100000, key_range); // Smaller test for scaling
        std::cout << std::left << std::setw(15) << size << std::setw(20)
                  << std::fixed << std::setprecision(0) << result.ops_per_second
                  << std::setw(15) << std::fixed << std::setprecision(1)
                  << (result.hit_rate * 100) << "%" << "\n";
    }
    std::cout << std::string(60, '-') << "\n";

    return 0;
}
