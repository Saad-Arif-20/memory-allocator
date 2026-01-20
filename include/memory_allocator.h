/**
 * @file memory_allocator.h
 * @brief Custom memory allocator implementation
 *
 * A simplified malloc/free implementation demonstrating memory management,
 * pointer manipulation, and data structures in C.
 */

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stdbool.h>
#include <stddef.h>


/**
 * Allocation strategies
 */
typedef enum {
  FIRST_FIT, /**< Allocate first available block */
  BEST_FIT,  /**< Allocate smallest sufficient block */
  WORST_FIT  /**< Allocate largest available block */
} AllocationStrategy;

/**
 * Memory block header structure
 * Placed at the beginning of each allocated/free block
 */
typedef struct BlockHeader {
  size_t size;              /**< Size of the block (excluding header) */
  bool is_free;             /**< Free status */
  struct BlockHeader *next; /**< Next block in the list */
} BlockHeader;

/**
 * Memory allocator statistics
 */
typedef struct {
  size_t total_memory;    /**< Total memory pool size */
  size_t used_memory;     /**< Currently allocated memory */
  size_t free_memory;     /**< Currently free memory */
  size_t num_allocations; /**< Total allocation calls */
  size_t num_frees;       /**< Total free calls */
  size_t num_blocks;      /**< Total number of blocks */
  size_t num_free_blocks; /**< Number of free blocks */
  double fragmentation;   /**< Fragmentation percentage */
} AllocatorStats;

/**
 * Initialize the memory allocator
 *
 * @param pool_size Size of memory pool in bytes
 * @param strategy Allocation strategy to use
 * @return true if successful, false otherwise
 */
bool allocator_init(size_t pool_size, AllocationStrategy strategy);

/**
 * Allocate memory block
 *
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL on failure
 */
void *my_malloc(size_t size);

/**
 * Free allocated memory block
 *
 * @param ptr Pointer to memory to free
 */
void my_free(void *ptr);

/**
 * Reallocate memory block
 *
 * @param ptr Pointer to existing block
 * @param new_size New size in bytes
 * @return Pointer to reallocated memory, or NULL on failure
 */
void *my_realloc(void *ptr, size_t new_size);

/**
 * Coalesce adjacent free blocks
 * Merges consecutive free blocks to reduce fragmentation
 */
void coalesce_free_blocks(void);

/**
 * Get allocator statistics
 *
 * @param stats Pointer to stats structure to fill
 */
void get_allocator_stats(AllocatorStats *stats);

/**
 * Print memory map showing all blocks
 */
void print_memory_map(void);

/**
 * Print allocator statistics
 */
void print_allocator_stats(void);

/**
 * Cleanup and free the memory pool
 */
void allocator_cleanup(void);

/**
 * Get string representation of allocation strategy
 *
 * @param strategy Allocation strategy
 * @return String representation
 */
const char *strategy_to_string(AllocationStrategy strategy);

#endif /* MEMORY_ALLOCATOR_H */
