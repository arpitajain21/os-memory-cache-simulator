#include "../../include/cache.h"
#include <iostream>

/*
 * Initializes cache structure.
 */
Cache::Cache(size_t cache_size, size_t block_size)
    : cache_size_bytes(cache_size),
      block_size_bytes(block_size),
      global_time(0),
      hit_count(0),
      miss_count(0) {

    number_of_lines = cache_size_bytes / block_size_bytes;
    cache_lines.resize(number_of_lines);
}

/*
 * Accesses cache using FIFO replacement.
 */
bool Cache::access(size_t address) {
    global_time++;

    size_t block_address = address / block_size_bytes;
    size_t line_index = block_address % number_of_lines;
    size_t tag = block_address / number_of_lines;

    CacheLine& line = cache_lines[line_index];

    // Cache hit
    if (line.valid && line.tag == tag) {
        hit_count++;
        return true;
    }

    // Cache miss → FIFO replacement
    miss_count++;
    line.valid = true;
    line.tag = tag;
    line.fifo_timestamp = global_time;

    return false;
}

/*
 * Prints cache statistics.
 */
void Cache::print_stats(const std::string& cache_name) const {
    int total_accesses = hit_count + miss_count;
    double hit_ratio =
        (total_accesses == 0) ? 0.0 :
        (double)hit_count / total_accesses;

    std::cout << "\n--- " << cache_name << " Cache Stats ---\n";
    std::cout << "Hits: " << hit_count << "\n";
    std::cout << "Misses: " << miss_count << "\n";
    std::cout << "Hit Ratio: " << hit_ratio * 100 << "%\n";
}