# Custom Memory Allocator - Systems Programming Project

A systems-level C programming project demonstrating manual memory management, low-level data structures, and operating system concepts. This project implements `malloc`, `free`, and `realloc` functionality with multiple allocation strategies.

## Purpose

This project demonstrates proficiency in:
- **C programming** (pointers, structures, manual memory management)
- **Systems programming** (low-level memory operations)
- **Data structures in C** (linked lists, block management)
- **Algorithm implementation** (First Fit, Best Fit, Worst Fit)
- **Memory management** (allocation, deallocation, fragmentation)
- **Performance analysis** (fragmentation tracking, statistics)

## Quick Start

### Prerequisites
- GCC compiler (MinGW on Windows, GCC on Linux/Mac)
- Make utility
- C11 standard library

### Compilation and Running

```bash
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
  MEMORY MAP
========================================

Block 0:
  Address: 0x...
  Size: 40 bytes
  Status: ALLOCATED
  Next: 0x...

Block 1:
  Address: 0x...
  Size: 56 bytes
  Status: ALLOCATED
  Next: 0x...

...

========================================
  ALLOCATOR STATISTICS
========================================

Strategy: First Fit
Total Memory: 10240 bytes
Used Memory: 296 bytes (2.9%)
Free Memory: 9944 bytes (97.1%)

Total Blocks: 4
Free Blocks: 1
Allocated Blocks: 3

Allocations: 3
Frees: 0
Fragmentation: 0.00%
```

## Core Concepts Demonstrated

### Memory Management
- **Custom Allocator**: Implementation of malloc/free from scratch
- **Block Headers**: Metadata for each memory block
- **Free List**: Linked list of available blocks
- **Block Splitting**: Dividing large blocks for smaller allocations
- **Block Coalescing**: Merging adjacent free blocks

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

## Project Structure

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

## Implementation Details

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

## Algorithm Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| **malloc (First Fit)** | O(n) | O(1) |
| **malloc (Best Fit)** | O(n) | O(1) |
| **malloc (Worst Fit)** | O(n) | O(1) |
| **free** | O(1) | O(1) |
| **coalesce** | O(n) | O(1) |
| **realloc** | O(n) | O(n) |

Where n = number of blocks

## Features

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

## Testing

### Unit Tests

The test suite covers:
- [x] Initialization and cleanup
- [x] Basic allocation and deallocation
- [x] Block reuse after free
- [x] Block coalescing
- [x] Reallocation
- [x] All three allocation strategies
- [x] Edge cases (zero size, NULL, double free)
- [x] Fragmentation tracking

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

## Performance Characteristics

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

## Learning Context & Academic Alignment

This project demonstrates applied understanding of key computer science concepts:

| Outcome | Implementation Evidence |
| :--- | :--- |
| **Systems Programming** | Complete C implementation with manual memory management and pointer arithmetic. |
| **Memory Management** | Implementation of malloc/free/realloc with three allocation strategies (First/Best/Worst Fit). |
| **Algorithm Design** | Block splitting, coalescing, and fragmentation tracking algorithms. |
| **Performance Analysis** | Complexity analysis and benchmarking of different allocation strategies. |

**Related Concepts**:
*   Systems programming concepts
*   Operating systems concepts
*   Data structures and algorithms

---


## Design Decisions

### Why Implement malloc/free?

**Educational Value**:
- Demonstrates understanding of memory management
- Shows low-level programming skills
- Illustrates algorithm trade-offs
- Provides hands-on systems programming experience

**Real-World Relevance**:
- Custom allocators used in embedded systems
- Game engines often implement custom memory management
- Performance-critical applications benefit from specialized allocators

### Why Three Strategies?

**Comparison and Analysis**:
- Shows understanding of algorithm trade-offs
- Demonstrates performance analysis skills
- Illustrates real-world decision-making
- Provides benchmarking experience

### Why Block Headers?

**Metadata Management**:
- Enables free list traversal
- Stores block size for validation
- Tracks allocation status
- Minimal overhead (one header per block)

## Limitations and Future Improvements

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

## Compilation Options

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
- Windows (MinGW/MSVC)
- Linux (GCC)
- macOS (Clang)

## Author

**SAAD ARIF**
**Year**: 2025

Aspiring Computer Science undergraduate (advanced entry)

Background in engineering and software development

## License

This project was created for educational and professional development purposes to consolidate and demonstrate core systems programming and memory management concepts.

## Acknowledgments

- Memory management concepts from operating systems literature
- C programming best practices from K&R
- Allocation algorithms from classic CS textbooks
- Systems programming techniques from Unix/Linux development

---

**Note**: This project demonstrates understanding of systems programming and memory management through practical implementation. All code is original and was developed for educational and professional skill consolidation purposes.
