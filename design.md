<body>

<h1>Design Document</h1>
<h2>OS Memory and Cache Simulator</h2>

<hr>

<h2>1. Project Overview</h2>
<p>
This project implements a simulator for physical memory management and multilevel CPU cache behavior using C++.
The design emphasizes modularity, clarity, and correctness while allowing users to experiment with memory allocation
strategies and cache performance through an interactive command-line interface.
</p>

<hr>

<h2>2. Physical Memory Representation</h2>

<h3>2.1 Data Structure</h3>
<p>
Physical memory is modeled as a doubly linked list of memory blocks, implemented using the <code>MemoryBlock</code>
structure. Each node represents a contiguous region of memory.
</p>

<ul>
    <li><b>start</b> – starting physical address</li>
    <li><b>size</b> – size of the block in bytes</li>
    <li><b>requested</b> – user-requested size (used for internal fragmentation)</li>
    <li><b>free</b> – allocation status</li>
    <li><b>block_id</b> – unique identifier for allocated blocks</li>
    <li><b>prev / next</b> – pointers for bidirectional traversal</li>
</ul>

<h3>2.2 Memory Layout Diagram</h3>
<pre>
Physical Memory (Contiguous)

+------------------------------------------------------+
|                  TOTAL MEMORY                        |
+------------------------------------------------------+
0                                                    1023
</pre>

After allocations:
<pre>
+-------------------+     +-------------------+     +-------------------+
| USED (id=1)       | --> | USED (id=2)       | --> | FREE              |
| start=0           |     | start=100         |     | start=300         |
| size=100          |     | size=200          |     | size=724          |
+-------------------+     +-------------------+     +-------------------+
</pre>

<hr>

<h2>3. Memory Initialization</h2>
<p>
Memory initialization is handled by <code>MemoryManager::init()</code>. Any existing memory blocks are deleted,
and a single free block covering the entire memory range is created. Allocation counters are reset to ensure
a clean state.
</p>

<hr>

<h2>4. Allocation Strategy Design</h2>

<h3>4.1 First Fit</h3>
<p>
The First Fit allocator traverses the memory block list from the head and selects the first free block whose size
is sufficient. If the block is larger than required, it is split into an allocated block and a residual free block.
</p>

<h3>4.2 Best Fit</h3>
<p>
The Best Fit allocator performs a full traversal of the list to locate the smallest free block that satisfies the
request. This block is split if necessary, reducing unused space within allocated blocks.
</p>

<h3>4.3 Worst Fit</h3>
<p>
The Worst Fit allocator selects the largest available free block during traversal. Allocation from larger blocks
allows observation of different fragmentation patterns.
</p>

<h3>Allocation & Splitting Diagram</h3>
<pre>
Before allocation:
+-------------------------------------------+
| FREE (start=100, size=300)                |
+-------------------------------------------+

After malloc(180):
+---------------------------+  +-------------------------+
| USED (id=4)               |  | FREE                    |
| start=100, size=180       |  | start=280, size=120     |
+---------------------------+  +-------------------------+
</pre>

<hr>

<h2>5. Deallocation and Coalescing</h2>
<p>
Deallocation is performed using <code>free_block(block_id)</code>. The block is marked free and merged with
adjacent free blocks using the doubly linked list structure.
</p>

<h3>Coalescing Diagram</h3>
<pre>
Before free:
+-----------+   +-----------+   +-----------+
| FREE      |   | USED      |   | FREE      |
| size=100  |   | size=200  |   | size=300  |
+-----------+   +-----------+   +-----------+

After free:
+--------------------------------------------+
| FREE                                      |
| start=0, size=600                          |
+--------------------------------------------+
</pre>

<hr>

<h2>6. Fragmentation and Metrics</h2>

<h3>Internal Fragmentation</h3>
<pre>
Internal Fragmentation =
Σ (allocated size − requested size)
</pre>

<h3>External Fragmentation</h3>
<pre>
External Fragmentation =
1 − (largest free block / total free memory)
</pre>

<h3>Memory Utilization</h3>
<pre>
Memory Utilization =
used memory / total memory
</pre>

Metrics are computed by traversing the memory block list.

<hr>

<h2>7. CLI Design</h2>
<p>
The CLI is implemented in <code>main.cpp</code> and serves as a controller layer. It parses user commands and
invokes appropriate memory management operations.
</p>

<p>Supported commands include:</p>
<pre>
init memory &lt;size&gt;
set allocator &lt;first_fit | best_fit | worst_fit&gt;
malloc &lt;size&gt;
free &lt;block_id&gt;
dump
stats
exit
</pre>

<hr>

<h2>8. Cache Simulation Design</h2>

<h3>8.1 Cache Structure</h3>
<p>
Each cache level is represented by a <code>Cache</code> class containing a vector of cache lines. Each line stores
a valid bit and a tag.
</p>

<h3>8.2 Cache Hierarchy</h3>
<pre>
CPU
 |
 v
+------------------+
| L1 Cache (64B)   |
+------------------+
 |
(L1 Miss)
 |
 v
+------------------+
| L2 Cache (128B)  |
+------------------+
 |
(L2 Miss)
 |
 v
Main Memory
</pre>

<h3>8.3 Address Mapping</h3>
<pre>
Physical Address
+----------------------------------+
| TAG | INDEX | OFFSET              |
+----------------------------------+
</pre>

Address mapping is implemented using modulo-based indexing.

<hr>

<h2>9. Replacement Policy</h2>
<p>
FIFO replacement is implemented implicitly. On an index collision, the existing cache line is overwritten.
This avoids maintaining access history and keeps the design simple.
</p>

<hr>

<h2>10. Virtual Memory (Conceptual Design)</h2>
<p>
Virtual memory is not implemented but is conceptually described to satisfy design requirements.
The intended model uses paging with page tables mapping virtual pages to physical frames.
</p>

<h3>Virtual Address Translation Flow</h3>
<pre>
Virtual Address
      |
      v
+------------------+
| Page Number      |
+------------------+
      |
      v
+------------------+
| Page Table       |
+------------------+
      |
      v
+------------------+
| Frame Number     |
+------------------+
      |
      v
Physical Address
</pre>

<hr>

<h2>11. Design Limitations</h2>
<ul>
    <li>No paging or page replacement algorithms</li>
    <li>Direct-mapped cache only</li>
    <li>No TLB simulation</li>
    <li>No multi-process memory sharing</li>
</ul>

<hr>

<h2>12. Conclusion</h2>
<p>
This project provides a clear and modular simulation of operating system memory management and cache behavior.
The design prioritizes understandability, correctness, and demonstrability through an interactive CLI.
</p>

</body>
</html>