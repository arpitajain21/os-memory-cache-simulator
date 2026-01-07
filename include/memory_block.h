#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <cstddef>

/*
 * Represents a contiguous block of physical memory.
 * Blocks are maintained as a doubly linked list.
 */
struct MemoryBlock {
    size_t start_address;      // Starting address of the block
    size_t block_size;         // Size of the block in bytes
    size_t requested_size;     // Size actually requested by user
    bool is_free;              // True if block is free, false if allocated
    int allocation_id;         // Unique ID for allocated block (-1 if free)

    MemoryBlock* prev;         // Previous block in memory list
    MemoryBlock* next;         // Next block in memory list

    MemoryBlock(size_t start, size_t size)
        : start_address(start),
          block_size(size),
          requested_size(0),
          is_free(true),
          allocation_id(-1),
          prev(nullptr),
          next(nullptr) {}
};

#endif