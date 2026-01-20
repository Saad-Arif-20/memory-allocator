/**
 * @file main.c
 * @brief Memory allocator demonstration program
 */

#include "memory_allocator.h"
#include <stdio.h>
#include <string.h>

#define POOL_SIZE (1024 * 10) /* 10 KB */

void print_header(const char *title) {
  printf("\n");
  printf("========================================\n");
  printf("  %s\n", title);
  printf("========================================\n");
}

void demo_basic_allocation(void) {
  print_header("BASIC ALLOCATION DEMO");

  printf("\nAllocating memory blocks...\n");

  /* Allocate some blocks */
  int *arr1 = (int *)my_malloc(10 * sizeof(int));
  char *str1 = (char *)my_malloc(50 * sizeof(char));
  double *arr2 = (double *)my_malloc(5 * sizeof(double));

  if (arr1 && str1 && arr2) {
    /* Use the allocated memory */
    for (int i = 0; i < 10; i++) {
      arr1[i] = i * 10;
    }
    strcpy(str1, "Hello from custom allocator!");
    for (int i = 0; i < 5; i++) {
      arr2[i] = i * 3.14;
    }

    printf("[+] Allocated and initialized 3 blocks\n");
    printf("    - int array[10]: %p\n", (void *)arr1);
    printf("    - char string[50]: %p -> \"%s\"\n", (void *)str1, str1);
    printf("    - double array[5]: %p\n", (void *)arr2);
  }

  print_memory_map();
  print_allocator_stats();

  /* Free some blocks */
  printf("\nFreeing middle block (string)...\n");
  my_free(str1);

  print_memory_map();
  print_allocator_stats();

  /* Free remaining blocks */
  printf("\nFreeing remaining blocks...\n");
  my_free(arr1);
  my_free(arr2);

  print_allocator_stats();
}

void demo_fragmentation(void) {
  print_header("FRAGMENTATION DEMO");

  printf("\nCreating fragmented memory...\n");

  /* Allocate multiple blocks */
  void *blocks[10];
  for (int i = 0; i < 10; i++) {
    blocks[i] = my_malloc(64);
    printf("[+] Allocated block %d: %p\n", i, blocks[i]);
  }

  print_allocator_stats();

  /* Free every other block to create fragmentation */
  printf("\nFreeing every other block...\n");
  for (int i = 0; i < 10; i += 2) {
    my_free(blocks[i]);
    printf("[-] Freed block %d\n", i);
  }

  print_memory_map();
  print_allocator_stats();

  /* Coalesce free blocks */
  printf("\nCoalescing free blocks...\n");
  coalesce_free_blocks();

  print_memory_map();
  print_allocator_stats();

  /* Free remaining blocks */
  for (int i = 1; i < 10; i += 2) {
    my_free(blocks[i]);
  }
}

void demo_reallocation(void) {
  print_header("REALLOCATION DEMO");

  printf("\nAllocating initial array...\n");
  int *arr = (int *)my_malloc(5 * sizeof(int));

  if (arr) {
    for (int i = 0; i < 5; i++) {
      arr[i] = i + 1;
    }
    printf("[+] Initial array: ");
    for (int i = 0; i < 5; i++) {
      printf("%d ", arr[i]);
    }
    printf("\n");
  }

  print_allocator_stats();

  printf("\nReallocating to larger size...\n");
  arr = (int *)my_realloc(arr, 10 * sizeof(int));

  if (arr) {
    for (int i = 5; i < 10; i++) {
      arr[i] = i + 1;
    }
    printf("[+] Reallocated array: ");
    for (int i = 0; i < 10; i++) {
      printf("%d ", arr[i]);
    }
    printf("\n");
  }

  print_allocator_stats();

  my_free(arr);
}

void demo_strategy_comparison(void) {
  print_header("ALLOCATION STRATEGY COMPARISON");

  AllocationStrategy strategies[] = {FIRST_FIT, BEST_FIT, WORST_FIT};
  const char *strategy_names[] = {"First Fit", "Best Fit", "Worst Fit"};

  for (int s = 0; s < 3; s++) {
    printf("\n--- Testing %s Strategy ---\n", strategy_names[s]);

    /* Reinitialize with current strategy */
    allocator_cleanup();
    allocator_init(POOL_SIZE, strategies[s]);

    /* Allocate blocks of varying sizes */
    void *b1 = my_malloc(100);
    void *b2 = my_malloc(200);
    void *b3 = my_malloc(50);
    void *b4 = my_malloc(150);

    printf("Allocated 4 blocks (100, 200, 50, 150 bytes)\n");

    /* Free some blocks */
    my_free(b2);
    my_free(b4);
    printf("Freed blocks 2 and 4\n");

    /* Try to allocate a block that fits in freed space */
    void *b5 = my_malloc(80);
    printf("Allocated new block (80 bytes)\n");

    AllocatorStats stats;
    get_allocator_stats(&stats);
    printf("\nResults:\n");
    printf("  Free Blocks: %zu\n", stats.num_free_blocks);
    printf("  Fragmentation: %.2f%%\n", stats.fragmentation);

    /* Cleanup */
    my_free(b1);
    my_free(b3);
    my_free(b5);
  }
}

int main(void) {
  printf("========================================\n");
  printf("  CUSTOM MEMORY ALLOCATOR\n");
  printf("  Demonstration Program\n");
  printf("========================================\n");

  /* Initialize allocator */
  printf("\nInitializing allocator with %d KB pool...\n", POOL_SIZE / 1024);
  if (!allocator_init(POOL_SIZE, FIRST_FIT)) {
    fprintf(stderr, "Failed to initialize allocator\n");
    return 1;
  }
  printf("[+] Allocator initialized successfully\n");

  /* Run demonstrations */
  demo_basic_allocation();

  /* Reinitialize for next demo */
  allocator_cleanup();
  allocator_init(POOL_SIZE, FIRST_FIT);
  demo_fragmentation();

  /* Reinitialize for next demo */
  allocator_cleanup();
  allocator_init(POOL_SIZE, FIRST_FIT);
  demo_reallocation();

  /* Strategy comparison */
  demo_strategy_comparison();

  /* Cleanup */
  print_header("CLEANUP");
  printf("\nCleaning up allocator...\n");
  allocator_cleanup();
  printf("[+] Cleanup complete\n");

  printf("\n========================================\n");
  printf("  DEMONSTRATION COMPLETE\n");
  printf("========================================\n\n");

  printf("Key Features Demonstrated:\n");
  printf("  [+] Custom malloc/free implementation\n");
  printf("  [+] Three allocation strategies\n");
  printf("  [+] Block splitting and coalescing\n");
  printf("  [+] Fragmentation tracking\n");
  printf("  [+] Memory reallocation\n");
  printf("  [+] Statistics and visualization\n");

  return 0;
}
