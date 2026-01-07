#include "../../include/memory_manager.h"
#include <iostream>

/*
 * Constructor initializes internal counters.
 */
MemoryManager::MemoryManager()
    : memory_head(nullptr),
      total_memory_size(0),
      next_allocation_id(1),
      total_requests(0),
      successful_allocations(0),
      failed_allocations(0) {}

/*
 * Initializes physical memory with a single free block.
 */
void MemoryManager::init(size_t memory_size) {
    // Delete existing memory blocks
    MemoryBlock* current = memory_head;
    while (current) {
        MemoryBlock* next = current->next;
        delete current;
        current = next;
    }

    memory_head = new MemoryBlock(0, memory_size);
    total_memory_size = memory_size;

    next_allocation_id = 1;
    total_requests = 0;
    successful_allocations = 0;
    failed_allocations = 0;
}

/*
 * First Fit allocation strategy.
 */
int MemoryManager::malloc_first_fit(size_t request_size) {
    total_requests++;

    MemoryBlock* current = memory_head;
    while (current) {
        if (current->is_free && current->block_size >= request_size) {

            // Split block if larger than required
            if (current->block_size > request_size) {
                MemoryBlock* remaining =
                    new MemoryBlock(current->start_address + request_size,
                                    current->block_size - request_size);

                remaining->next = current->next;
                if (current->next)
                    current->next->prev = remaining;

                current->next = remaining;
                remaining->prev = current;
            }

            current->block_size = request_size;
            current->requested_size = request_size;
            current->is_free = false;
            current->allocation_id = next_allocation_id++;

            successful_allocations++;
            return current->allocation_id;
        }
        current = current->next;
    }

    failed_allocations++;
    return -1;
}

/*
 * Best Fit allocation strategy.
 */
int MemoryManager::malloc_best_fit(size_t request_size) {
    total_requests++;

    MemoryBlock* best_block = nullptr;
    MemoryBlock* current = memory_head;

    while (current) {
        if (current->is_free && current->block_size >= request_size) {
            if (!best_block || current->block_size < best_block->block_size)
                best_block = current;
        }
        current = current->next;
    }

    if (!best_block) {
        failed_allocations++;
        return -1;
    }

    if (best_block->block_size > request_size) {
        MemoryBlock* remaining =
            new MemoryBlock(best_block->start_address + request_size,
                            best_block->block_size - request_size);

        remaining->next = best_block->next;
        if (best_block->next)
            best_block->next->prev = remaining;

        best_block->next = remaining;
        remaining->prev = best_block;
    }

    best_block->block_size = request_size;
    best_block->requested_size = request_size;
    best_block->is_free = false;
    best_block->allocation_id = next_allocation_id++;

    successful_allocations++;
    return best_block->allocation_id;
}

/*
 * Worst Fit allocation strategy.
 */
int MemoryManager::malloc_worst_fit(size_t request_size) {
    total_requests++;

    MemoryBlock* worst_block = nullptr;
    MemoryBlock* current = memory_head;

    while (current) {
        if (current->is_free && current->block_size >= request_size) {
            if (!worst_block || current->block_size > worst_block->block_size)
                worst_block = current;
        }
        current = current->next;
    }

    if (!worst_block) {
        failed_allocations++;
        return -1;
    }

    if (worst_block->block_size > request_size) {
        MemoryBlock* remaining =
            new MemoryBlock(worst_block->start_address + request_size,
                            worst_block->block_size - request_size);

        remaining->next = worst_block->next;
        if (worst_block->next)
            worst_block->next->prev = remaining;

        worst_block->next = remaining;
        remaining->prev = worst_block;
    }

    worst_block->block_size = request_size;
    worst_block->requested_size = request_size;
    worst_block->is_free = false;
    worst_block->allocation_id = next_allocation_id++;

    successful_allocations++;
    return worst_block->allocation_id;
}

/*
 * Frees an allocated memory block and coalesces neighbors.
 */
void MemoryManager::free_block(int allocation_id) {
    MemoryBlock* current = memory_head;

    while (current && current->allocation_id != allocation_id)
        current = current->next;

    if (!current || current->is_free) {
        std::cout << "Invalid block id\n";
        return;
    }

    current->is_free = true;
    current->allocation_id = -1;
    current->requested_size = 0;

    // Merge with previous block
    if (current->prev && current->prev->is_free) {
        MemoryBlock* prev = current->prev;
        prev->block_size += current->block_size;
        prev->next = current->next;
        if (current->next)
            current->next->prev = prev;
        delete current;
        current = prev;
    }

    // Merge with next block
    if (current->next && current->next->is_free) {
        MemoryBlock* next = current->next;
        current->block_size += next->block_size;
        current->next = next->next;
        if (next->next)
            next->next->prev = current;
        delete next;
    }
}

/*
 * Prints current memory layout.
 */
void MemoryManager::dump_memory() const {
    MemoryBlock* current = memory_head;
    std::cout << "----- Memory Dump -----\n";
    while (current) {
        std::cout << "[" << current->start_address << " - "
                  << current->start_address + current->block_size - 1 << "] ";
        if (current->is_free)
            std::cout << "FREE";
        else
            std::cout << "USED (id=" << current->allocation_id << ")";
        std::cout << "\n";
        current = current->next;
    }
}

/*
 * Computes and prints fragmentation and utilization metrics.
 */
void MemoryManager::print_metrics() const {
    size_t total_free_memory = 0;
    size_t largest_free_block = 0;
    size_t internal_fragmentation = 0;
    size_t used_memory = 0;

    MemoryBlock* current = memory_head;
    while (current) {
        if (current->is_free) {
            total_free_memory += current->block_size;
            if (current->block_size > largest_free_block)
                largest_free_block = current->block_size;
        } else {
            used_memory += current->block_size;
            internal_fragmentation +=
                (current->block_size - current->requested_size);
        }
        current = current->next;
    }

    double external_fragmentation =
        (total_free_memory == 0) ? 0.0 :
        1.0 - (double)largest_free_block / total_free_memory;

    double utilization =
        (double)used_memory / total_memory_size;

    std::cout << "\n--- Metrics ---\n";
    std::cout << "Total Memory: " << total_memory_size << "\n";
    std::cout << "Used Memory: " << used_memory << "\n";
    std::cout << "Free Memory: " << total_free_memory << "\n";
    std::cout << "Internal Fragmentation: " << internal_fragmentation << "\n";
    std::cout << "External Fragmentation: " << external_fragmentation << "\n";
    std::cout << "Memory Utilization: " << utilization * 100 << "%\n";
    std::cout << "Total Requests: " << total_requests << "\n";
    std::cout << "Successful Allocs: " << successful_allocations << "\n";
    std::cout << "Failed Allocs: " << failed_allocations << "\n";
}
