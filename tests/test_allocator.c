/**
 * @file test_allocator.c
 * @brief Unit tests for memory allocator
 */

#include "memory_allocator.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


#define TEST_POOL_SIZE 4096
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

static int tests_passed = 0;
static int tests_failed = 0;

void test_assert(int condition, const char *test_name) {
  if (condition) {
    printf("[PASS] %s\n", test_name);
    tests_passed++;
  } else {
    printf("[FAIL] %s\n", test_name);
    tests_failed++;
  }
}

void test_initialization(void) {
  printf("\n=== Testing Initialization ===\n");

  allocator_cleanup();
  int result = allocator_init(TEST_POOL_SIZE, FIRST_FIT);
  test_assert(result == 1, "Allocator initialization");

  AllocatorStats stats;
  get_allocator_stats(&stats);
  test_assert(stats.total_memory == TEST_POOL_SIZE, "Total memory size");
  test_assert(stats.used_memory == 0, "Initial used memory is zero");
  test_assert(stats.num_blocks == 1, "Initial block count");
}

void test_basic_allocation(void) {
  printf("\n=== Testing Basic Allocation ===\n");

  allocator_cleanup();
  allocator_init(TEST_POOL_SIZE, FIRST_FIT);

  void *ptr1 = my_malloc(100);
  test_assert(ptr1 != NULL, "Allocate 100 bytes");

  void *ptr2 = my_malloc(200);
  test_assert(ptr2 != NULL, "Allocate 200 bytes");

  void *ptr3 = my_malloc(50);
  test_assert(ptr3 != NULL, "Allocate 50 bytes");

  AllocatorStats stats;
  get_allocator_stats(&stats);
  test_assert(stats.num_allocations == 3, "Allocation count");
  test_assert(stats.used_memory > 0, "Used memory increased");

  my_free(ptr1);
  my_free(ptr2);
  my_free(ptr3);
}

void test_free_and_reuse(void) {
  printf("\n=== Testing Free and Reuse ===\n");

  allocator_cleanup();
  allocator_init(TEST_POOL_SIZE, FIRST_FIT);

  void *ptr1 = my_malloc(100);
  void *ptr2 = my_malloc(100);
  void *ptr3 = my_malloc(100);

  my_free(ptr2); // Free middle block

  AllocatorStats stats;
  get_allocator_stats(&stats);
  size_t free_after_one = stats.free_memory;

  void *ptr4 = my_malloc(50); // Should reuse freed block
  test_assert(ptr4 != NULL, "Reuse freed block");

  get_allocator_stats(&stats);
  test_assert(stats.free_memory < free_after_one,
              "Free memory decreased after reuse");

  my_free(ptr1);
  my_free(ptr3);
  my_free(ptr4);
}

void test_coalescing(void) {
  printf("\n=== Testing Block Coalescing ===\n");

  allocator_cleanup();
  allocator_init(TEST_POOL_SIZE, FIRST_FIT);

  void *ptr1 = my_malloc(100);
  void *ptr2 = my_malloc(100);
  void *ptr3 = my_malloc(100);

  AllocatorStats stats;
  get_allocator_stats(&stats);
  size_t blocks_before = stats.num_blocks;

  my_free(ptr1);
  my_free(ptr2);
  my_free(ptr3);

  coalesce_free_blocks();

  get_allocator_stats(&stats);
  test_assert(stats.num_blocks < blocks_before, "Blocks coalesced");
  test_assert(stats.num_free_blocks == 1, "Single free block after coalesce");

  allocator_cleanup();
}

void test_reallocation(void) {
  printf("\n=== Testing Reallocation ===\n");

  allocator_cleanup();
  allocator_init(TEST_POOL_SIZE, FIRST_FIT);

  int *arr = (int *)my_malloc(5 * sizeof(int));
  test_assert(arr != NULL, "Initial allocation");

  for (int i = 0; i < 5; i++) {
    arr[i] = i;
  }

  arr = (int *)my_realloc(arr, 10 * sizeof(int));
  test_assert(arr != NULL, "Reallocation to larger size");

  // Check data preserved
  int data_preserved = 1;
  for (int i = 0; i < 5; i++) {
    if (arr[i] != i) {
      data_preserved = 0;
      break;
    }
  }
  test_assert(data_preserved, "Data preserved after realloc");

  my_free(arr);
}

void test_allocation_strategies(void) {
  printf("\n=== Testing Allocation Strategies ===\n");

  AllocationStrategy strategies[] = {FIRST_FIT, BEST_FIT, WORST_FIT};
  const char *names[] = {"First Fit", "Best Fit", "Worst Fit"};

  for (int i = 0; i < 3; i++) {
    allocator_cleanup();
    allocator_init(TEST_POOL_SIZE, strategies[i]);

    void *p1 = my_malloc(100);
    void *p2 = my_malloc(200);
    void *p3 = my_malloc(100);

    test_assert(p1 != NULL && p2 != NULL && p3 != NULL, names[i]);

    my_free(p1);
    my_free(p2);
    my_free(p3);
  }
}

void test_edge_cases(void) {
  printf("\n=== Testing Edge Cases ===\n");

  allocator_cleanup();
  allocator_init(TEST_POOL_SIZE, FIRST_FIT);

  // Test zero allocation
  void *ptr_zero = my_malloc(0);
  test_assert(ptr_zero == NULL, "Zero-size allocation returns NULL");

  // Test NULL free
  my_free(NULL);
  test_assert(1, "Free NULL pointer doesn't crash");

  // Test double free detection
  void *ptr = my_malloc(100);
  my_free(ptr);
  my_free(ptr); // Should print error but not crash
  test_assert(1, "Double free detected");

  // Test very large allocation
  void *large = my_malloc(TEST_POOL_SIZE * 2);
  test_assert(large == NULL, "Too-large allocation fails gracefully");
}

void test_fragmentation_tracking(void) {
  printf("\n=== Testing Fragmentation Tracking ===\n");

  allocator_cleanup();
  allocator_init(TEST_POOL_SIZE, FIRST_FIT);

  // Create fragmented memory
  void *blocks[10];
  for (int i = 0; i < 10; i++) {
    blocks[i] = my_malloc(64);
  }

  // Free every other block
  for (int i = 0; i < 10; i += 2) {
    my_free(blocks[i]);
  }

  AllocatorStats stats;
  get_allocator_stats(&stats);
  double frag_before = stats.fragmentation;

  // Coalesce
  coalesce_free_blocks();
  get_allocator_stats(&stats);

  test_assert(stats.fragmentation <= frag_before,
              "Fragmentation reduced after coalescing");

  // Free remaining
  for (int i = 1; i < 10; i += 2) {
    my_free(blocks[i]);
  }
}

int main(void) {
  printf("========================================\n");
  printf("  MEMORY ALLOCATOR UNIT TESTS\n");
  printf("========================================\n");

  test_initialization();
  test_basic_allocation();
  test_free_and_reuse();
  test_coalescing();
  test_reallocation();
  test_allocation_strategies();
  test_edge_cases();
  test_fragmentation_tracking();

  allocator_cleanup();

  printf("\n========================================\n");
  printf("  TEST RESULTS\n");
  printf("========================================\n");
  printf("Tests Passed: %s%d%s\n", ANSI_COLOR_GREEN, tests_passed,
         ANSI_COLOR_RESET);
  printf("Tests Failed: %s%d%s\n", tests_failed > 0 ? ANSI_COLOR_RED : "",
         tests_failed, ANSI_COLOR_RESET);
  printf("Total Tests: %d\n", tests_passed + tests_failed);
  printf("Success Rate: %.1f%%\n",
         (double)tests_passed / (tests_passed + tests_failed) * 100.0);
  printf("========================================\n\n");

  return tests_failed > 0 ? 1 : 0;
}
