#include "../include/memory_manager.h"
#include "../include/cache.h"
#include <iostream>
#include <sstream>
#include <string>

/*
 * CLI-based memory simulator entry point.
 */

enum AllocatorType {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

int main() {
    MemoryManager memory_manager;
    AllocatorType current_allocator = FIRST_FIT;

    std::string input_line;
    std::cout << "Memory Simulator CLI\n";
    std::cout << "Type 'exit' to quit\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input_line);
        if (input_line.empty()) continue;

        std::stringstream parser(input_line);
        std::string command;
        parser >> command;

        if (command == "exit") {
            break;
        }

        else if (command == "init") {
            std::string keyword;
            size_t size;
            parser >> keyword >> size;
            if (keyword == "memory") {
                memory_manager.init(size);
                std::cout << "Initialized memory of size " << size << "\n";
            }
        }

        else if (command == "set") {
            std::string keyword, type;
            parser >> keyword >> type;
            if (keyword == "allocator") {
                if (type == "first_fit") current_allocator = FIRST_FIT;
                else if (type == "best_fit") current_allocator = BEST_FIT;
                else if (type == "worst_fit") current_allocator = WORST_FIT;
                else {
                    std::cout << "Unknown allocator type\n";
                    continue;
                }
                std::cout << "Allocator set to " << type << "\n";
            }
        }

        else if (command == "malloc") {
            size_t size;
            parser >> size;
            int allocation_id = -1;

            if (current_allocator == FIRST_FIT)
                allocation_id = memory_manager.malloc_first_fit(size);
            else if (current_allocator == BEST_FIT)
                allocation_id = memory_manager.malloc_best_fit(size);
            else
                allocation_id = memory_manager.malloc_worst_fit(size);

            if (allocation_id == -1)
                std::cout << "Allocation failed\n";
            else
                std::cout << "Allocated block id=" << allocation_id << "\n";
        }

        else if (command == "free") {
            int allocation_id;
            parser >> allocation_id;
            memory_manager.free_block(allocation_id);
            std::cout << "Block " << allocation_id << " freed\n";
        }

        else if (command == "dump") {
            memory_manager.dump_memory();
        }

        else if (command == "stats") {
            memory_manager.print_metrics();
        }

        else {
            std::cout << "Unknown command\n";
        }
    }
        // ---------- CACHE SIMULATION DEMO ----------
    std::cout << "\nRunning Cache Simulation...\n";

    Cache L1(64, 16);   // 64B cache, 16B block size
    Cache L2(128, 16);  // 128B cache, 16B block size

    size_t addresses[] = {0, 4, 8, 16, 32, 0, 4, 64, 128, 0};

    for (size_t addr : addresses) {
        if (!L1.access(addr)) {
            L2.access(addr);
        }
    }

    L1.print_stats("L1");
    L2.print_stats("L2");

    return 0;
}