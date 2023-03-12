#include <assert.h>
#include "list.h"
#include "pagetable_generic.h"

#include "pagetable_generic.h"

#define CACHE_CAPACITY 1024

static struct list_head* cache;
static unsigned int size;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int
lru_evict(void)
{
  // evict the tail node; add at head node
  // int frame = container_of(cache->head.prev,pt_entry_t,frame);
  int frame = cache->head.prev
  list_del(cache->head.prev);
  assert(false);
  return -1;
}

/* This function is called on each access to a page to update any information
 * needed by the LRU algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void
lru_ref(int frame)
{
  // list_for_each()
  list_for_each
  (void)frame;
}

/* Initialize any data structures needed for this replacement algorithm. */
void
lru_init(void)
{
  cache =(list_head*)malloc(sizeof(list_head));
  list_init(cache);
  size = 0;
}

/* Cleanup any data structures created in lru_init(). */
void
lru_cleanup(void)
{}
