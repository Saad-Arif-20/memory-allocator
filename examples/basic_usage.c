/**
 * @file basic_usage.c
 * @brief Simple example of using the custom memory allocator
 */

#include "../include/memory_allocator.h"
#include <stdio.h>

int main(void) {
  printf("=== Basic Usage Example ===\n\n");

  // 1. Initialize the allocator with 1024 bytes (1 KB)
  printf("Initializing 1KB memory pool...\n");
  if (!allocator_init(1024, FIRST_FIT)) {
    fprintf(stderr, "Initialization failed!\n");
    return 1;
  }

  // 2. Allocate memory
  printf("Allocating variables...\n");
  int *number = (int *)my_malloc(sizeof(int));
  char *text = (char *)my_malloc(20 * sizeof(char));

  // 3. Use memory
  if (number && text) {
    *number = 42;
    snprintf(text, 20, "Hello, World!");

    printf("Stored number: %d\n", *number);
    printf("Stored text: %s\n", text);
  }

  // 4. View statistics
  print_allocator_stats();

  // 5. Free memory
  printf("Freeing memory...\n");
  my_free(number);
  my_free(text);

  print_allocator_stats();

  // 6. Cleanup
  allocator_cleanup();
  printf("\nExample complete.\n");
  return 0;
}
