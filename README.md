<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>OS Memory & Cache Simulator</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            line-height: 1.6;
        }
        h1, h2, h3 {
            color: #2c3e50;
        }
        code, pre {
            background-color: #f4f4f4;
            padding: 6px;
            display: block;
            border-radius: 4px;
        }
        ul {
            margin-left: 20px;
        }
    </style>
</head>

<body>

<h1>OS Memory & Cache Simulator</h1>

<p>
A C++ based simulator that models <strong>physical memory management</strong> and
<strong>multilevel CPU cache behavior</strong>, developed as part of an Operating Systems project.
The simulator provides an interactive <strong>CLI interface</strong> to experiment with memory
allocation strategies, fragmentation metrics, and cache hit/miss behavior.
</p>

<hr>

<h2>Features Implemented</h2>

<h3>1. Physical Memory Simulation</h3>
<ul>
    <li>Simulates a contiguous block of physical memory</li>
    <li>Configurable memory size</li>
    <li>Memory represented in bytes</li>
    <li>Dynamic splitting and coalescing of blocks</li>
</ul>

<h3>2. Memory Allocation Strategies</h3>
<ul>
    <li>First Fit</li>
    <li>Best Fit</li>
    <li>Worst Fit</li>
</ul>

<p>
Each allocation request searches for a suitable free block and splits it if required.
Deallocation merges adjacent free blocks to reduce fragmentation.
</p>

<h3>3. CLI Interface</h3>
<p>Supported commands:</p>

<pre>
init memory &lt;size&gt;
set allocator &lt;first_fit | best_fit | worst_fit&gt;
malloc &lt;size&gt;
free &lt;block_id&gt;
dump
stats
exit
</pre>

<h3>4. Metrics & Statistics</h3>
<ul>
    <li>Internal Fragmentation</li>
    <li>External Fragmentation</li>
    <li>Memory Utilization</li>
    <li>Allocation Success / Failure Count</li>
</ul>

<h3>5. Multilevel Cache Simulation</h3>
<ul>
    <li>L1 Cache</li>
    <li>L2 Cache</li>
    <li>Direct-mapped cache</li>
    <li>FIFO replacement policy</li>
</ul>

<p>
Cache hit/miss statistics and hit ratio are displayed after cache simulation.
</p>

<hr>

<h2>Project Structure</h2>

<pre>
os-memory-cache-simulator/
│
├── include/
│   ├── memory_block.h
│   ├── memory_manager.h
│   └── cache.h
│
├── src/
│   ├── memory/
│   │   ├── memory_manager.cpp
│   │   └── cache.cpp
│   └── main.cpp
│
├── docs/
│   └── screenshots/
│
├── README.html
└── .gitignore
</pre>

<hr>

<h2>Build Instructions</h2>

<h3>Requirements</h3>
<ul>
    <li>C++ compiler with C++17 support</li>
    <li>g++ / MinGW / GCC / Clang</li>
</ul>

<h3>Compile</h3>
<pre>
g++ -std=c++17 src/memory/memory_manager.cpp src/memory/cache.cpp src/main.cpp -I include -o memsim
</pre>

<h3>Run</h3>
<pre>
./memsim        (Linux / macOS)
.\memsim        (Windows)
</pre>

<hr>

<h2>Demonstration & Test Artifacts</h2>
<p>
Screenshots demonstrating allocation behavior, cache hits/misses,
and error handling are stored in:
</p>

<pre>
docs/screenshots/
</pre>

<hr>

<h2>Design Documentation</h2>
<p>
A separate design document explains the internal working of:
</p>
<ul>
    <li>Memory layout and assumptions</li>
    <li>Allocation strategy logic</li>
    <li>Fragmentation metrics</li>
    <li>Cache hierarchy and replacement policy</li>
    <li>Virtual memory model (conceptual)</li>
</ul>

<hr>

<h2>Limitations</h2>
<ul>
    <li>Virtual memory is described conceptually but not fully simulated</li>
    <li>Cache is direct-mapped only</li>
    <li>FIFO replacement policy is implemented</li>
</ul>

<hr>

<h2>Author</h2>
<p>
<strong>Arpita Jain</strong><br>
Design and Implementation of a Memory Management Simulator<br>
IIT Roorkee
</p>

</body>
</html>
