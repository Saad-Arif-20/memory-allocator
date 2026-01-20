/**
 * @file performance_test.c
 * @brief Compare fragmentation between different allocation strategies
 */

#include "../include/memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POOL_SIZE (1024 * 10) // 10 KB
#define NUM_ALLOCS 50

void run_test(AllocationStrategy strategy, const char *name) {
  printf("\nTesting %s Strategy:\n", name);

  if (!allocator_init(POOL_SIZE, strategy)) {
    printf("Failed to init\n");
    return;
  }

  void *pointers[NUM_ALLOCS];

  // Simulate random allocation patterns
  for (int i = 0; i < NUM_ALLOCS; i++) {
    // Allocate random size between 10 and 100 bytes
    size_t size = 10 + (rand() % 90);
    pointers[i] = my_malloc(size);
  }

  // Free every other block to create fragmentation holes
  for (int i = 0; i < NUM_ALLOCS; i += 2) {
    if (pointers[i]) {
      my_free(pointers[i]);
    }
  }

  // Try to allocate a large block in the fragmented heap
  void *large_block = my_malloc(150);
  if (large_block) {
    printf("  [+] Successfully allocated large block (150 bytes)\n");
    my_free(large_block);
  } else {
    printf("  [-] Failed to allocate large block due to fragmentation\n");
  }

  AllocatorStats stats;
  get_allocator_stats(&stats);
  printf("  Final Fragmentation: %.2f%%\n", stats.fragmentation);
  printf("  Free Blocks: %zu\n", stats.num_free_blocks);

  allocator_cleanup();
}

int main(void) {
  srand((unsigned int)time(NULL));
  printf("=== Performance & Fragmentation Test ===\n");

  run_test(FIRST_FIT, "First Fit");
  run_test(BEST_FIT, "Best Fit ");
  run_test(WORST_FIT, "Worst Fit");

  return 0;
}
