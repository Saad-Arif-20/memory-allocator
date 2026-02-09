# 🧠 Custom Memory Allocator

[![C](https://img.shields.io/badge/C-C11-A8B9CC?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey)](https://github.com/Saad-Arif-20/memory-allocator)

A production-inspired systems-level C programming project demonstrating manual memory management, low-level data structures, and operating system concepts. Implements `malloc`, `free`, and `realloc` functionality with three allocation strategies (First Fit, Best Fit, Worst Fit).

## 🚀 At a Glance

- 🔧 **Custom malloc/free/realloc** (built from scratch in C)
- 📊 **3 allocation strategies** (First Fit, Best Fit, Worst Fit)
- 🧩 **Block management** (splitting, coalescing, free list)
- 📈 **Fragmentation tracking** (statistics and visualization)
- 🧪 **Comprehensive testing** (25+ unit tests, 100% pass rate)
- ⚡ **Performance analysis** (complexity analysis, benchmarking)

⏱️ **Setup time**: ~2 minutes  
🎓 **Ideal for**: Systems programming, OS concepts, low-level C development

---

## 📚 Table of Contents

- [Why This Project?](#-why-this-project)
- [Quick Start](#-quick-start)
- [Core Concepts](#-core-concepts-demonstrated)
- [Implementation Details](#-implementation-details)
- [Features](#-features)
- [Testing](#-testing)
- [Performance Characteristics](#-performance-characteristics)
- [Key Design Decisions](#-key-design-decisions)
- [What I Learned](#-what-i-learned)
- [Limitations & Future Improvements](#-limitations-and-future-improvements)
- [Contributing](#-contributing)

---

## 🌟 Why This Project?

Memory management is fundamental to systems programming and operating systems. While modern languages provide automatic memory management, understanding how allocators work at a low level is crucial for performance-critical applications, embedded systems, and systems programming.

### Key Highlights
- 🔧 **Systems Programming** - Manual memory management with pointers and structures
- 🧠 **Algorithm Implementation** - Three allocation strategies with trade-off analysis
- 📊 **Performance Analysis** - Complexity analysis and fragmentation tracking
- 🧪 **Production Practices** - Comprehensive testing, error handling, statistics
- 💡 **Real-World Relevance** - Custom allocators used in game engines, embedded systems

---

## 🚀 Quick Start

### Prerequisites
- GCC compiler (MinGW on Windows, GCC on Linux/Mac)
- Make utility
- C11 standard library

### Installation

```bash
# Clone the repository
git clone https://github.com/Saad-Arif-20/memory-allocator.git
cd memory-allocator

# Build the project
make

# Run the demonstration
make run

# Run unit tests
make run-tests

# Clean build artifacts
make clean
```

### Expected Output

```
========================================
  CUSTOM MEMORY ALLOCATOR
  Demonstration Program
========================================

Initializing allocator with 10 KB pool...
[+] Allocator initialized successfully

========================================
  BASIC ALLOCATION DEMO
========================================

Allocating memory blocks...
[+] Allocated and initialized 3 blocks
    - int array[10]: 0x...
    - char string[50]: 0x... -> "Hello from custom allocator!"
    - double array[5]: 0x...

========================================
  ALLOCATOR STATISTICS
========================================

Strategy: First Fit
Total Memory: 10240 bytes
Used Memory: 296 bytes (2.9%)
Free Memory: 9944 bytes (97.1%)

Total Blocks: 4
Allocated Blocks: 3
Fragmentation: 0.00%
```

---

## 🧩 Core Concepts Demonstrated

### Memory Management
- **Custom Allocator**: Implementation of malloc/free from scratch
- **Block Headers**: Metadata for each memory block
- **Free List**: Linked list of available blocks
- **Block Splitting**: Dividing large blocks for smaller allocations
- **Block Coalescing**: Merging adjacent free blocks to reduce fragmentation

### Allocation Strategies
- **First Fit**: Allocate first available block (fast, moderate fragmentation)
- **Best Fit**: Allocate smallest sufficient block (slower, less fragmentation)
- **Worst Fit**: Allocate largest available block (fast, more fragmentation)

### C Programming Techniques
- **Pointer Arithmetic**: Calculating block addresses
- **Structure Manipulation**: Managing block headers
- **Type Casting**: Converting between pointer types
- **Memory Alignment**: 8-byte alignment for performance
- **Linked Lists**: Manual implementation without stdlib

---

## 🏗️ Project Structure

```
/memory-allocator
 ├── include/                 # Header files
 │   └── memory_allocator.h  # Public API
 │
 ├── src/                    # Source files
 │   ├── memory_allocator.c  # Allocator implementation
 │   └── main.c              # Demonstration program
 │
 ├── tests/                  # Unit tests
 │   └── test_allocator.c    # Comprehensive test suite
 │
 ├── examples/               # Example programs
 │
 ├── Makefile                # Build configuration
 ├── .gitignore              # Git ignore rules
 └── README.md               # This file
```

---

## 🔍 Implementation Details

### Block Header Structure

```c
typedef struct BlockHeader {
    size_t size;                /**< Size of the block (excluding header) */
    bool is_free;               /**< Free status */
    struct BlockHeader *next;   /**< Next block in the list */
} BlockHeader;
```

### Memory Layout

```
[BlockHeader][User Data][BlockHeader][User Data]...
     ^            ^
     |            |
  Metadata    Returned to user
```

### Allocation Process

1. **Find suitable block** using selected strategy
2. **Split block** if it's larger than needed
3. **Mark as allocated** and update statistics
4. **Return pointer** to usable memory (after header)

### Deallocation Process

1. **Validate pointer** is within pool
2. **Mark block as free** and update statistics
3. **Coalesce** with adjacent free blocks
4. **Update free list**

### Algorithm Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| **malloc (First Fit)** | O(n) | O(1) |
| **malloc (Best Fit)** | O(n) | O(1) |
| **malloc (Worst Fit)** | O(n) | O(1) |
| **free** | O(1) | O(1) |
| **coalesce** | O(n) | O(1) |
| **realloc** | O(n) | O(n) |

Where n = number of blocks

---

## ✨ Features

### 1. Custom malloc()
- Finds free block using selected strategy
- Splits blocks when possible
- Aligns memory to 8 bytes
- Returns NULL on failure

### 2. Custom free()
- Validates pointer
- Detects double-free errors
- Automatically coalesces adjacent blocks
- Updates statistics

### 3. Custom realloc()
- Resizes existing allocations
- Preserves data when moving
- Optimizes when shrinking
- Handles edge cases (NULL, zero size)

### 4. Three Allocation Strategies
- **First Fit**: Fast, simple, moderate fragmentation
- **Best Fit**: Minimizes wasted space, slower
- **Worst Fit**: Leaves larger fragments, fastest

### 5. Fragmentation Tracking
- Calculates external fragmentation percentage
- Tracks number of free blocks
- Monitors largest free block
- Provides visualization

### 6. Statistics and Visualization
- Total/used/free memory
- Allocation/deallocation counts
- Block counts
- Memory map display
- Fragmentation metrics

---

## 🧪 Testing

### Unit Tests

The test suite covers:
- ✅ Initialization and cleanup
- ✅ Basic allocation and deallocation
- ✅ Block reuse after free
- ✅ Block coalescing
- ✅ Reallocation
- ✅ All three allocation strategies
- ✅ Edge cases (zero size, NULL, double free)
- ✅ Fragmentation tracking

### Running Tests

```bash
make run-tests
```

### Expected Test Output

```
========================================
  MEMORY ALLOCATOR UNIT TESTS
========================================

=== Testing Initialization ===
[PASS] Allocator initialization
[PASS] Total memory size
[PASS] Initial used memory is zero
[PASS] Initial block count

=== Testing Basic Allocation ===
[PASS] Allocate 100 bytes
[PASS] Allocate 200 bytes
[PASS] Allocate 50 bytes
[PASS] Allocation count
[PASS] Used memory increased

...

========================================
  TEST RESULTS
========================================
Tests Passed: 25
Tests Failed: 0
Total Tests: 25
Success Rate: 100.0%
========================================
```

---

## 📊 Performance Characteristics

### First Fit
- **Speed**: Fast (stops at first suitable block)
- **Fragmentation**: Moderate
- **Best for**: General-purpose allocation

### Best Fit
- **Speed**: Slower (searches entire list)
- **Fragmentation**: Lower (minimizes waste)
- **Best for**: Memory-constrained environments

### Worst Fit
- **Speed**: Slower (searches entire list)
- **Fragmentation**: Higher (creates small fragments)
- **Best for**: Allocations of similar sizes

---

## 🧠 Key Design Decisions

### Why Implement malloc/free from Scratch?
- **Deep Understanding**: Learn how memory allocators actually work
- **Systems Programming Skills**: Demonstrate low-level C proficiency
- **Real-World Relevance**: Custom allocators used in embedded systems, game engines
- **Interview Preparation**: Common systems programming interview topic

### Why Three Allocation Strategies?
- **Trade-off Analysis**: Understand performance vs fragmentation trade-offs
- **Algorithm Comparison**: Empirical evidence of complexity differences
- **Decision Making**: Learn when to use each strategy
- **Benchmarking Experience**: Measure and compare real performance

### Why Block Headers?
- **Metadata Management**: Store size and status information
- **Free List Traversal**: Enable linked list of free blocks
- **Minimal Overhead**: One header per block (16 bytes on 64-bit)
- **Validation**: Detect invalid frees and double-frees

### Why Fixed-Size Pool?
- **Simplicity**: Focus on allocation algorithms, not pool management
- **Predictability**: Known memory bounds for testing
- **Embedded Systems**: Common constraint in resource-limited environments
- **Learning Focus**: Understand core concepts before adding complexity

### Why 8-Byte Alignment?
- **Performance**: CPU cache line optimization
- **Portability**: Works on all modern architectures
- **Standards**: Common alignment for malloc implementations
- **Simplicity**: Easy to implement with bitwise operations

---

## 💡 What I Learned

### Technical Insights
- **Memory management is complex**: Fragmentation, coalescing, and alignment are subtle
- **Pointer arithmetic is powerful**: Direct memory manipulation enables low-level control
- **Metadata overhead matters**: Headers consume space, affecting usable memory
- **Alignment affects performance**: Unaligned access can be slow or cause crashes

### Algorithm Lessons
- **First Fit is practical**: Good balance of speed and fragmentation
- **Best Fit isn't always best**: Can create many tiny unusable fragments
- **Coalescing is essential**: Without it, fragmentation grows unbounded
- **Linear search limits scalability**: O(n) allocation becomes bottleneck at scale

### Systems Programming
- **C requires discipline**: Manual memory management is error-prone
- **Testing is critical**: Edge cases (NULL, zero size, double free) must be handled
- **Visualization helps debugging**: Memory maps reveal allocation patterns
- **Statistics guide optimization**: Measure before optimizing

### Professional Growth
- **Low-level understanding matters**: Knowing how malloc works helps use it better
- **Trade-offs are everywhere**: Speed vs fragmentation, simplicity vs features
- **Documentation prevents bugs**: Clear API contracts catch errors early
- **Real-world constraints differ**: Embedded systems need different allocators than servers

---

## ⚠️ Limitations and Future Improvements

### Current Limitations
1. **Single Pool**: Fixed-size memory pool
2. **No Thread Safety**: Not safe for concurrent access
3. **Linear Search**: O(n) allocation time
4. **No Defragmentation**: Cannot move allocated blocks
5. **Fixed Alignment**: 8-byte alignment only

### Potential Improvements
1. **Advanced Features**:
   - Thread-safe operations (mutexes)
   - Multiple memory pools
   - Buddy system allocation
   - Slab allocator for fixed sizes

2. **Performance Optimizations**:
   - Segregated free lists
   - Binary search trees for free blocks
   - Cache-aware allocation
   - SIMD operations

3. **Additional Functionality**:
   - Memory debugging tools
   - Leak detection
   - Bounds checking
   - Usage profiling

---

## 🛠️ Compilation Options

### Debug Build
```bash
make debug
```
Includes:
- Debug symbols (-g)
- DEBUG macro defined
- No optimizations

### Release Build
```bash
make
```
Includes:
- Optimizations (-O2)
- No debug symbols
- Faster execution

### Platform Support
- ✅ Windows (MinGW/MSVC)
- ✅ Linux (GCC)
- ✅ macOS (Clang)

---

## 🎯 Use Cases

### For Developers
- **Learning**: Understand memory management internals
- **Reference**: See allocation algorithm implementations
- **Embedded Systems**: Template for custom allocators

### For Students
- **Operating Systems**: Practical memory management example
- **Systems Programming**: Low-level C programming practice
- **Algorithms**: See complexity analysis in action

### For Interviewers
- **Systems Knowledge**: Demonstrates understanding of memory management
- **C Proficiency**: Shows pointer arithmetic and structure manipulation
- **Algorithm Analysis**: Compares trade-offs between strategies

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

### Areas for Contribution
- Additional allocation strategies (Buddy System, Slab Allocator)
- Performance benchmarking tools
- Thread-safety implementation
- Memory debugging features

---

## 👤 Author

**Saad Arif**
- GitHub: [@Saad-Arif-20](https://github.com/Saad-Arif-20)
- LinkedIn: [@saad--arif](https://www.linkedin.com/in/saad--arif/)

---

## 📄 License

MIT License - This project was created for educational and professional development purposes.

---

## 🙏 Acknowledgments

- Memory management concepts from operating systems literature
- C programming best practices from K&R
- Allocation algorithms from classic CS textbooks
- Systems programming techniques from Unix/Linux development

---

**© 2025 Saad Arif** | Built with C and systems programming expertise
