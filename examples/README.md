# Examples

This directory contains example programs demonstrating different use cases of the custom memory allocator.

## Available Examples

### 1. Basic Usage (`basic_usage.c`)
Demonstrates the fundamental operations:
- Initializing the allocator
- Allocating integers and strings (`my_malloc`)
- Using the allocated memory
- Freeing memory (`my_free`)
- Viewing statistics

### 2. String Processing (`string_processing.c`)
Shows a real-world use case handling dynamic resizing:
- Allocating an initial string buffer
- Using `my_realloc` to expand the buffer as data grows
- Concatenating strings safely

### 3. Performance Test (`performance_test.c`)
A benchmark tool that:
- Runs simulations with random allocation patterns
- Compares fragmentation across policies (First Fit, Best Fit, Worst Fit)
- Creates memory holes to stress-test the allocator

## How to Compile and Run

You can build all examples from the project root using:

```bash
make examples
```

Then run them individually:

```bash
./build/basic_usage.exe
./build/string_processing.exe
./build/performance_test.exe
```
