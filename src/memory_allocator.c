/**
 * @file memory_allocator.c
 * @brief Implementation of custom memory allocator
 */

#include "memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables */
static void *memory_pool = NULL;
static BlockHeader *head = NULL;
static size_t pool_size = 0;
static AllocationStrategy current_strategy = FIRST_FIT;
static AllocatorStats stats = {0};

/**
 * Initialize the memory allocator
 */
bool allocator_init(size_t size, AllocationStrategy strategy) {
  if (memory_pool != NULL) {
    fprintf(stderr, "Allocator already initialized\n");
    return false;
  }

  if (size < sizeof(BlockHeader)) {
    fprintf(stderr, "Pool size too small\n");
    return false;
  }

  /* Allocate memory pool */
  memory_pool = malloc(size);
  if (memory_pool == NULL) {
    fprintf(stderr, "Failed to allocate memory pool\n");
    return false;
  }

  pool_size = size;
  current_strategy = strategy;

  /* Initialize first block */
  head = (BlockHeader *)memory_pool;
  head->size = size - sizeof(BlockHeader);
  head->is_free = true;
  head->next = NULL;

  /* Initialize statistics */
  stats.total_memory = size;
  stats.free_memory = head->size;
  stats.used_memory = 0;
  stats.num_blocks = 1;
  stats.num_free_blocks = 1;
  stats.num_allocations = 0;
  stats.num_frees = 0;
  stats.fragmentation = 0.0;

  return true;
}

/**
 * Find free block using First Fit strategy
 */
static BlockHeader *find_first_fit(size_t size) {
  BlockHeader *current = head;

  while (current != NULL) {
    if (current->is_free && current->size >= size) {
      return current;
    }
    current = current->next;
  }

  return NULL;
}

/**
 * Find free block using Best Fit strategy
 */
static BlockHeader *find_best_fit(size_t size) {
  BlockHeader *current = head;
  BlockHeader *best = NULL;
  size_t best_size = SIZE_MAX;

  while (current != NULL) {
    if (current->is_free && current->size >= size) {
      if (current->size < best_size) {
        best = current;
        best_size = current->size;
      }
    }
    current = current->next;
  }

  return best;
}

/**
 * Find free block using Worst Fit strategy
 */
static BlockHeader *find_worst_fit(size_t size) {
  BlockHeader *current = head;
  BlockHeader *worst = NULL;
  size_t worst_size = 0;

  while (current != NULL) {
    if (current->is_free && current->size >= size) {
      if (current->size > worst_size) {
        worst = current;
        worst_size = current->size;
      }
    }
    current = current->next;
  }

  return worst;
}

/**
 * Find free block based on current strategy
 */
static BlockHeader *find_free_block(size_t size) {
  switch (current_strategy) {
  case FIRST_FIT:
    return find_first_fit(size);
  case BEST_FIT:
    return find_best_fit(size);
  case WORST_FIT:
    return find_worst_fit(size);
  default:
    return find_first_fit(size);
  }
}

/**
 * Split block if it's large enough
 */
static void split_block(BlockHeader *block, size_t size) {
  /* Only split if remaining space is large enough for a new block */
  if (block->size >= size + sizeof(BlockHeader) + 1) {
    BlockHeader *new_block = (BlockHeader *)((char *)(block + 1) + size);
    new_block->size = block->size - size - sizeof(BlockHeader);
    new_block->is_free = true;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;

    stats.num_blocks++;
    stats.num_free_blocks++;
  }
}

/**
 * Allocate memory block
 */
void *my_malloc(size_t size) {
  if (memory_pool == NULL) {
    fprintf(stderr, "Allocator not initialized\n");
    return NULL;
  }

  if (size == 0) {
    return NULL;
  }

  /* Align size to 8 bytes */
  size = (size + 7) & ~7;

  /* Find suitable free block */
  BlockHeader *block = find_free_block(size);

  if (block == NULL) {
    fprintf(stderr, "No suitable block found for size %zu\n", size);
    return NULL;
  }

  /* Split block if possible */
  split_block(block, size);

  /* Mark block as allocated */
  block->is_free = false;

  /* Update statistics */
  stats.num_allocations++;
  stats.used_memory += block->size;
  stats.free_memory -= block->size;
  stats.num_free_blocks--;

  /* Return pointer to usable memory (after header) */
  return (void *)(block + 1);
}

/**
 * Free allocated memory block
 */
void my_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  if (memory_pool == NULL) {
    fprintf(stderr, "Allocator not initialized\n");
    return;
  }

  /* Get block header */
  BlockHeader *block = (BlockHeader *)ptr - 1;

  /* Validate block is within pool */
  if ((void *)block < memory_pool ||
      (void *)block >= (void *)((char *)memory_pool + pool_size)) {
    fprintf(stderr, "Invalid pointer\n");
    return;
  }

  if (block->is_free) {
    fprintf(stderr, "Double free detected\n");
    return;
  }

  /* Mark block as free */
  block->is_free = true;

  /* Update statistics */
  stats.num_frees++;
  stats.used_memory -= block->size;
  stats.free_memory += block->size;
  stats.num_free_blocks++;

  /* Coalesce with adjacent free blocks */
  coalesce_free_blocks();
}

/**
 * Reallocate memory block
 */
void *my_realloc(void *ptr, size_t new_size) {
  if (ptr == NULL) {
    return my_malloc(new_size);
  }

  if (new_size == 0) {
    my_free(ptr);
    return NULL;
  }

  /* Get current block */
  BlockHeader *block = (BlockHeader *)ptr - 1;

  /* If new size fits in current block, just return */
  if (block->size >= new_size) {
    return ptr;
  }

  /* Allocate new block */
  void *new_ptr = my_malloc(new_size);
  if (new_ptr == NULL) {
    return NULL;
  }

  /* Copy data */
  memcpy(new_ptr, ptr, block->size);

  /* Free old block */
  my_free(ptr);

  return new_ptr;
}

/**
 * Coalesce adjacent free blocks
 */
void coalesce_free_blocks(void) {
  BlockHeader *current = head;

  while (current != NULL && current->next != NULL) {
    if (current->is_free && current->next->is_free) {
      /* Merge with next block */
      current->size += sizeof(BlockHeader) + current->next->size;
      current->next = current->next->next;

      stats.num_blocks--;
      stats.num_free_blocks--;
    } else {
      current = current->next;
    }
  }
}

/**
 * Calculate fragmentation percentage
 */
static double calculate_fragmentation(void) {
  if (stats.free_memory == 0) {
    return 0.0;
  }

  BlockHeader *current = head;
  size_t largest_free = 0;

  while (current != NULL) {
    if (current->is_free && current->size > largest_free) {
      largest_free = current->size;
    }
    current = current->next;
  }

  if (largest_free == 0) {
    return 0.0;
  }

  return ((double)(stats.free_memory - largest_free) / stats.free_memory) *
         100.0;
}

/**
 * Get allocator statistics
 */
void get_allocator_stats(AllocatorStats *out_stats) {
  if (out_stats == NULL) {
    return;
  }

  stats.fragmentation = calculate_fragmentation();
  *out_stats = stats;
}

/**
 * Print memory map
 */
void print_memory_map(void) {
  if (memory_pool == NULL) {
    printf("Allocator not initialized\n");
    return;
  }

  printf("\n========================================\n");
  printf("  MEMORY MAP\n");
  printf("========================================\n\n");

  BlockHeader *current = head;
  int block_num = 0;

  while (current != NULL) {
    printf("Block %d:\n", block_num++);
    printf("  Address: %p\n", (void *)current);
    printf("  Size: %zu bytes\n", current->size);
    printf("  Status: %s\n", current->is_free ? "FREE" : "ALLOCATED");
    printf("  Next: %p\n", (void *)current->next);
    printf("\n");

    current = current->next;
  }
}

/**
 * Print allocator statistics
 */
void print_allocator_stats(void) {
  stats.fragmentation = calculate_fragmentation();

  printf("\n========================================\n");
  printf("  ALLOCATOR STATISTICS\n");
  printf("========================================\n\n");

  printf("Strategy: %s\n", strategy_to_string(current_strategy));
  printf("Total Memory: %zu bytes\n", stats.total_memory);
  printf("Used Memory: %zu bytes (%.1f%%)\n", stats.used_memory,
         (double)stats.used_memory / stats.total_memory * 100.0);
  printf("Free Memory: %zu bytes (%.1f%%)\n", stats.free_memory,
         (double)stats.free_memory / stats.total_memory * 100.0);
  printf("\n");
  printf("Total Blocks: %zu\n", stats.num_blocks);
  printf("Free Blocks: %zu\n", stats.num_free_blocks);
  printf("Allocated Blocks: %zu\n", stats.num_blocks - stats.num_free_blocks);
  printf("\n");
  printf("Allocations: %zu\n", stats.num_allocations);
  printf("Frees: %zu\n", stats.num_frees);
  printf("Fragmentation: %.2f%%\n", stats.fragmentation);
  printf("\n");
}

/**
 * Cleanup allocator
 */
void allocator_cleanup(void) {
  if (memory_pool != NULL) {
    free(memory_pool);
    memory_pool = NULL;
    head = NULL;
    pool_size = 0;
    memset(&stats, 0, sizeof(stats));
  }
}

/**
 * Get string representation of strategy
 */
const char *strategy_to_string(AllocationStrategy strategy) {
  switch (strategy) {
  case FIRST_FIT:
    return "First Fit";
  case BEST_FIT:
    return "Best Fit";
  case WORST_FIT:
    return "Worst Fit";
  default:
    return "Unknown";
  }
}
