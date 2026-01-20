/**
 * @file string_processing.c
 * @brief Dynamic string processing using custom allocator
 */

#include "../include/memory_allocator.h"
#include <stdio.h>
#include <string.h>

int main(void) {
  printf("=== Dynamic String Processing ===\n\n");
  allocator_init(4096, FIRST_FIT); // 4 KB pool

  // Start with a small string
  char *buffer = (char *)my_malloc(15 * sizeof(char));
  if (!buffer)
    return 1;
  strcpy(buffer, "Hello");
  printf("Initial: %s (at %p)\n", buffer, (void *)buffer);

  // Reallocate to add more text
  printf("Expanding buffer...\n");
  buffer = (char *)my_realloc(buffer, 30 * sizeof(char));
  strcat(buffer, ", World!");
  printf("Updated: %s (at %p)\n", buffer, (void *)buffer);

  // Reallocate again for more
  printf("Expanding buffer again...\n");
  buffer = (char *)my_realloc(buffer, 60 * sizeof(char));
  strcat(buffer, " Welcome to Custom Memory Management.");
  printf("Final:   %s (at %p)\n", buffer, (void *)buffer);

  print_allocator_stats();

  my_free(buffer);
  allocator_cleanup();
  return 0;
}
