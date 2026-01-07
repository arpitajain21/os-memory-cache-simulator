#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "memory_block.h"
#include <cstddef>

/*
 * MemoryManager simulates physical memory allocation
 * using First Fit, Best Fit, and Worst Fit strategies.
 */
class MemoryManager {
private:
    MemoryBlock* memory_head;      // Head of memory block list
    size_t total_memory_size;      // Total physical memory size
    int next_allocation_id;        // Generates unique allocation IDs

    int total_requests;            // Total allocation requests
    int successful_allocations;    // Successful allocations
    int failed_allocations;        // Failed allocations

public:
    MemoryManager();

    void init(size_t memory_size);

    int malloc_first_fit(size_t request_size);
    int malloc_best_fit(size_t request_size);
    int malloc_worst_fit(size_t request_size);

    void free_block(int allocation_id);
    void dump_memory() const;
    void print_metrics() const;
};

#endif