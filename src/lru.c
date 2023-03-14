#include <assert.h>

#include "pagetable_generic.h"
#include "sim.h"
#include "pagetable.h"

#define CACHE_CAPACITY 1024

static int frame_head=0;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int
lru_evict(void)
{
  // evict the tail node; add at head node
  int frame = -1;
  struct frame head = coremap[frame_head];

  struct frame tail = *head.prev;

  if(&head == &tail){
    // only head node
    return frame_head;
  }

  struct frame tail_pre = * tail.prev;

  //return the tail frame
  frame = tail.pte->frame;

  //remove the tail node
  head.prev = & tail_pre;
  tail_pre.next = &head;
  tail.prev = NULL;
  tail.next = NULL;


  

  
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
  if(frame == frame_head) return;
  struct frame node = coremap[frame];
  //current head
  struct frame head = coremap[frame_head];

  if(node.prev == NULL) return;
  struct frame node_pre = *node.prev;

  //offload the node
  node_pre.next = node.next;
  node.next->prev = &node_pre;
  //add node in the front of the head
  node.next = & head;
  node.prev = head.prev;
  //link the head and node 
  head.prev = & node;
  //set the new head frame
  frame_head = frame;
}

/* Initialize any data structures needed for this replacement algorithm. */
void
lru_init(void)
{
  for(unsigned int i=1; i<memsize-1; i++){
    coremap[i].prev = &coremap[i-1];
    coremap[i].next = &coremap[i+1];
  }

  coremap[0].next = &coremap[1];
  coremap[0].prev = &coremap[memsize-1];
  
  coremap[memsize-1].prev = &coremap[memsize-2];
  coremap[memsize-1].next = &coremap[0];
}

/* Cleanup any data structures created in lru_init(). */
void
lru_cleanup(void)
{}
