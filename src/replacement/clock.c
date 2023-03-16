#include <assert.h>

#include "pagetable_generic.h"
#include "sim.h"


/* Page to evict is chosen using the CLOCK algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

static unsigned int hand;

int
clock_evict(void)
{
  // assert(hand <= memsize);
   while(true) {
      struct frame* cur_frame = &coremap[hand];
        if(cur_frame->is_exist) {
            if(get_referenced(cur_frame->pte)) {
                set_referenced(cur_frame->pte,false);
            }else {
                cur_frame->is_exist = false;
                return hand;
            }
        }
        hand = (hand+1) % memsize;
    }
}

/* This function is called on each access to a page to update any information
 * needed by the CLOCK algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void
clock_ref(int frame)
{ 
	// coremap[frame].is_ref = true;
  coremap[frame].is_exist = true;
  set_referenced(coremap[frame].pte,true);

}

/* Initialize any data structures needed for this replacement algorithm. */
void
clock_init(void)
{
  for(size_t i=0;i<memsize;i++){
    coremap[i].is_exist = false;
  }
  hand = 0;
}

/* Cleanup any data structures created in clock_init(). */
void
clock_cleanup(void)
{}
