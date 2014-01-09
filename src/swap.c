#include "vm/swap.h"
#include <bitmap.h>
#include <debug.h>
#include <stdio.h>
#include "vm/frame.h"
#include "vm/page.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

/* The swap device. */
static struct block *swap_device;

/* Used swap pages. */
static struct bitmap *swap_bitmap;

/* Protects swap_bitmap. */
static struct lock swap_lock;

/* Number of sectors per page. */
#define PAGE_SECTORS (PGSIZE / BLOCK_SECTOR_SIZE)
#define SWAPF 0
/* Sets up swap. */
void
swap_init (void) 
{
swap_device=block_get_role(BLOCK_SWAP);
if(swap_device!=NULL)
  {
   swap_bitmap=bitmap_create(block_size(swap_device)/PAGE_SECTORS);
  }
   lock_init(&swap_lock);

}

/* Swaps in page P, which must have a locked frame
   (and be swapped out). */
void
swap_in (struct page *p) 
{
size_t i;
for(i=0;i<PAGE_SECTORS;i++)
   {
    block_read(swap_device, 
p->sector+i,p->frame->base+i*BLOCK_SECTOR_SIZE);

   }

}

/* Swaps out page P, which must have a locked frame. */
bool
swap_out (struct page *p) 
{
 size_t index,j;



p->sector=index*PAGE_SECTORS;
for(j=0;j<PAGE_SECTORS;j++)
  
block_write(swap_device,p->sector+j,j*BLOCK_SECTOR_SIZE);


return true;



}
