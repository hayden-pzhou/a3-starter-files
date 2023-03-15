#include <assert.h>

#include <stdio.h>
#include "pagetable_generic.h"
#include "sim.h"
#include "pagetable.h"
#include <stdlib.h>

#define CACHE_CAPACITY 1024

struct frame * lru_head;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int
lru_evict(void)
{
  // evict the tail node; add at head node
  int frame = -1;

  //empty
  if(lru_head->prev == lru_head && lru_head->next == lru_head) return frame;

  struct frame* tail = lru_head->prev;
  tail->is_exist = false;
  frame = tail->pte->frame;
  frame_list_delete(tail);

  // assert(false);
  return frame;
}

/* This function is called on each access to a page to update any information
 * needed by the LRU algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void
lru_ref(int frame)
{
  struct frame* f = &coremap[frame];

  //first offload the entry
  if(f->is_exist)
    frame_list_delete(f);
  //append at head
  frame_list_insert(f,lru_head,lru_head->next);
  f->is_exist = true;

  return;
  
}

/* Initialize any data structures needed for this replacement algorithm. */
void
lru_init(void)
{
  lru_head =(struct frame*) malloc(sizeof(struct frame));
  frame_list_init_head(lru_head);

   for(unsigned int i=1; i<memsize-1; i++){
    coremap[i].is_exist = false;
    coremap[i].prev = &coremap[i-1];
    coremap[i].next = &coremap[i+1];
  }

  coremap[0].is_exist = false;
  coremap[0].next = &coremap[1];
  coremap[0].prev = &coremap[memsize-1];
  
  coremap[memsize-1].is_exist=false;
  coremap[memsize-1].prev = &coremap[memsize-2];
  coremap[memsize-1].next = &coremap[0];

}

/* Cleanup any data structures created in lru_init(). */
void
lru_cleanup(void)
{}
