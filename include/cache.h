#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstddef>
#include <string>

/*
 * Represents a single cache line.
 */
struct CacheLine {
    bool valid;
    size_t tag;
    size_t fifo_timestamp;   // Used for FIFO replacement

    CacheLine() : valid(false), tag(0), fifo_timestamp(0) {}
};

/*
 * Direct-mapped cache with FIFO replacement policy.
 */
class Cache {
private:
    size_t cache_size_bytes;
    size_t block_size_bytes;
    size_t number_of_lines;

    std::vector<CacheLine> cache_lines;

    size_t global_time;   // Monotonic counter for FIFO
    int hit_count;
    int miss_count;

public:
    Cache(size_t cache_size, size_t block_size);

    bool access(size_t address);
    void print_stats(const std::string& cache_name) const;
};

#endif