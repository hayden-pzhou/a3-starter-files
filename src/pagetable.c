/*
 * This code is provided solely for the personal and private use of students
 * taking the CSC369H course at the University of Toronto. Copying for purposes
 * other than this use is expressly prohibited. All forms of distribution of
 * this code, including but not limited to public repositories on GitHub,
 * GitLab, Bitbucket, or any other online platform, whether as given or with
 * any changes, are expressly prohibited.
 *
 * Authors: Andrew Peterson, Karen Reid, Alexey Khrabrov
 *
 * All of the files in this directory and all subdirectories are:
 * Copyright (c) 2019, 2021 Karen Reid
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagetable.h"
#include "pagetable_generic.h"
#include "sim.h"
#include "swap.h"

// Counters for various events.
// Your code must increment these when the related events occur.
size_t hit_count = 0;
size_t miss_count = 0;
size_t ref_count = 0;
size_t evict_clean_count = 0;
size_t evict_dirty_count = 0;

pdpt_entry_t* pdpt_list;
pd_entry_t * pd_list;
pt_entry_t* pagetable;

/*
 * Allocates a frame to be used for the virtual page represented by p.
 * If all frames are in use, calls the replacement algorithm's evict_func to
 * select a victim frame. Writes victim to swap if needed, and updates
 * page table entry for victim to indicate that virtual page is no longer in
 * (simulated) physical memory.
 *
 * Counters for evictions should be updated appropriately in this function.
 */
static int
allocate_frame(pt_entry_t* pte)
{
  int frame = -1;
  for (size_t i = 0; i < memsize; ++i) {
    if (!coremap[i].in_use) {
      frame = i;
      break;
    }
  }

  if (frame == -1) { // Didn't find a free page.
    // Call replacement algorithm's evict function to select victim
    frame = evict_func(); // virtual frame
    assert(frame != -1);

    // All frames were in use, so victim frame must hold some page
    // Write victim page to swap, if needed, and update page table

    // IMPLEMENTATION NEEDED
   if(is_dirty(coremap[frame].pte)){
      // if the frame is dirty
      off_t swap_offset = swap_pageout(coremap[frame].pte->frame,coremap[frame].pte->swap_offset);
      coremap[frame].pte->swap_offset = swap_offset;
      SET_PAGE(coremap[frame].pte,PAGE_ONSWAP);
      ++evict_dirty_count;
      if(swap_offset == INVALID_SWAP){
        fprintf(stderr, "dirty page swapin error!, frame id: %d, swap offset %ld\n", coremap[frame].pte->frame, coremap[frame].pte->swap_offset);
        exit(0);
      }
      }
      /*
      else if(!is_dirty(coremap[frame].pte)&&!is_onswap()){

      }
      */
      else {
        ++evict_clean_count;
      }
    // page is no longer valid, it is evicted
    set_frame_valid(coremap[frame].pte,false);
    // do we need to reset a swapout page to clean?
    // UNSET_PAGE(coremap[frame].pte,PAGE_DIRTY);
  }

  // set new frame valid and clean
  UNSET_PAGE(pte,PAGE_DIRTY);
  pte->frame = frame;
  
  // Record information for virtual page that will now be stored in frame
  coremap[frame].in_use = true;
  coremap[frame].pte = pte;

  return frame;
}

/*
 * Initializes your page table.
 * This function is called once at the start of the simulation.
 * For the simulation, there is a single "process" whose reference trace is
 * being simulated, so there is just one overall page table.
 *
 * In a real OS, each process would have its own page table, which would
 * need to be allocated and initialized as part of process creation.
 *
 * The format of the page table, and thus what you need to do to get ready
 * to start translating virtual addresses, is up to you.
 */
void
init_pagetable(void)
{
  //init the third levels page entry pointer
  pdpt_list = (pdpt_entry_t*) malloc (sizeof(pdpt_entry_t)*PTRS_PER_PDPT);
  memset(pdpt_list,0,sizeof(pdpt_entry_t)*PTRS_PER_PDPT);
  for(int i=0;i<PTRS_PER_PDPT;i++){
    pdpt_list[i].pdp = i;
  }

  pd_list = (pd_entry_t*) malloc (sizeof(pd_entry_t)*PTRS_PER_PD);
  memset(pd_list,0,sizeof(pd_entry_t*)*PTRS_PER_PD);
  for(int i=0;i<PTRS_PER_PD;i++){
    pd_list[i].pde = i;
  }
  
  pagetable = (pt_entry_t*) malloc (sizeof(pt_entry_t)*PTRS_PER_PT);
  memset(pagetable,0,sizeof(pt_entry_t)*PTRS_PER_PT);
  for(int i=0;i<PTRS_PER_PT;i++){
    pagetable[i].swap_offset=INVALID_SWAP;
  }

}

/*
 * Initializes the content of a (simulated) physical memory frame when it
 * is first allocated for some virtual address. Just like in a real OS, we
 * fill the frame with zeros to prevent leaking information across pages.
 */
static void
init_frame(int frame)
{
  // Calculate pointer to start of frame in (simulated) physical memory
  unsigned char* mem_ptr = &physmem[frame * SIMPAGESIZE];
  memset(mem_ptr, 0, SIMPAGESIZE); // zero-fill the frame
}

/*
 * Locate the physical frame number for the given vaddr using the page table.
 *
 * If the page table entry is invalid and not on swap, then this is the first
 * reference to the page and a (simulated) physical frame should be allocated
 * and initialized to all zeros (using init_frame).
 *
 * If the page table entry is invalid and on swap, then a (simulated) physical
 * frame should be allocated and filled by reading the page data from swap.
 *
 * When you have a valid page table entry, return the start of the page frame
 * that holds the requested virtual page.
 *
 * Counters for hit, miss and reference events should be incremented in
 * this function.
 */
unsigned char*
find_physpage(vaddr_t vaddr, char type)
{
  int frame = -1; // Frame used to hold vaddr

  // IMPLEMENTATION NEEDED
  // Use your page table to find the page table entry (pte) for the
  // requested vaddr.

  int pt_entry_i = PT_INDEX(vaddr);
  int pd_entry_i = PD_INDEX(vaddr);
  int pdpt_entry_i = PDPT_INDEX(vaddr);
  
  // translate vaddr to physical frame
  uintptr_t pdp =pdpt_list[pdpt_entry_i].pdp;

  uintptr_t pde = pd_list[pdp+pd_entry_i].pde;

  pt_entry_t *pet = &pagetable[pde+pt_entry_i];

  if(!is_valid(pet)&&!is_onswap(pet)){
    // not valid and not on the swap file, first time to reference, set page dirty
    ++miss_count;
    
    frame = allocate_frame(pet);
    init_frame(pet->frame); //allocate a frame from
    SET_PAGE(pet,PAGE_DIRTY);
    set_frame_valid(pet,true);
  }else if(!is_valid(pet)&&is_onswap(pet)){
    // not valid but on the swapfile, allocated a frame and swapin
    frame = allocate_frame(pet);
    init_frame(pet->frame);
    int ret = swap_pagein(pet->frame,pet->swap_offset);
    if(ret!=0){
      fprintf(stderr,"page swap error!, partial read %d Bytes\n",ret);
      exit(-1);
    }
    set_frame_valid(pet,true);
    ++miss_count;
  }else {
    // hit
    hit_count++;
  }
  ref_count++;
  frame = pet->frame;
  
  if ((type == 'S') || (type == 'M')) {
    SET_PAGE(pet,PAGE_DIRTY);
  }

  // Make sure that pte is marked valid and referenced. Also mark it
  // dirty if the access type indicates that the page will be written to.
  // (Note that a page should be marked DIRTY when it is first accessed,
  // even if the type of first access is a read (Load or Instruction type).

  // Call replacement algorithm's ref_func for this page.
  assert(frame != -1);
  ref_func(frame);


  // Return pointer into (simulated) physical memory at start of frame
  return &physmem[frame * SIMPAGESIZE];
}

void
print_pagetable(void)
{
  for(size_t i=0; i< memsize;i++){
    fprintf(stdout,"page entry:%ld, page is in use: %d, pte->flag %c, pte phisical frame %d, pte swapoffset %ld"
                ,i,coremap[i].in_use,coremap[i].pte->flag,coremap[i].pte->frame,coremap[i].pte->swap_offset);
  }
}

void
free_pagetable(void)
{
}

/**
 * @brief check page is valid
 * 
 * @param pte 
 * @return true is valid
 * @return false not valid, mean not in physical memory.
 */

/**
 * @brief check page is dirty
 * 
 * @param pte 
 * @return true page is dirty 
 * @return false page is clean
 */
bool
is_dirty(struct pt_entry_s* pte){
  return CHECK_PAGE(pte,PAGE_DIRTY);
}

bool
get_referenced(struct pt_entry_s* pte){
  return CHECK_PAGE(pte,PAGE_REF);
}

void
set_referenced(struct pt_entry_s* pte, bool val){
  if(val)
    SET_PAGE(pte,PAGE_REF);
  else
    UNSET_PAGE(pte,PAGE_REF);
}
/**
 * @brief check page is swap
 * 
 * @param pte 
 * @return true on swap file
 * @return false not on swap file
 */
bool
is_onswap(struct pt_entry_s* pte){
  return CHECK_PAGE(pte,PAGE_ONSWAP); // pageonswap bit : 1 mean on swap
}

void set_frame_valid(struct pt_entry_s* pte,bool val){
  if(val){
    SET_PAGE(pte,PAGE_VALID);
    }else{
    UNSET_PAGE(pte,PAGE_VALID);
  }
}
/**
 * @brief check frame is valid
 * 
 * @param pte 
 * @return true frame is valid, page is holding in physical memory
 * @return false frame is not valid, page is not holding in physical memory 
 */
bool
is_valid(struct pt_entry_s* pte){
  return CHECK_PAGE(pte,PAGE_VALID);
}