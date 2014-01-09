#include "vm/frame.h"
#include <stdio.h>
#include "vm/page.h"
#include "devices/timer.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
static size_t frcount;
static struct frame *f1[1000];

struct lock framelock;
void
frame_init (void) 
{

list_init(&frametable);
lock_init(&framelock);
}




/* Tries to allocate and lock a frame for PAGE.
   Returns the frame if successful, false on failure. */
struct frame *
frame_alloc_and_lock (struct page *page) 
{
void *temp;

    temp=palloc_get_page(PAL_USER);
   if(temp!=NULL)
     {
      struct frame *f=malloc(sizeof(struct frame));
      lock_init(&f->lock);
      f->page=page;
      f->base=temp;
     lock_acquire(&framelock);
     list_push_front(&frametable,&f->elem);
     lock_release(&framelock);
      return f;
     }
   
      
struct list_elem *e1;
e1=list_begin(&frametable);

while(1)
{
lock_acquire(&framelock);
struct frame *fr;
fr=malloc(sizeof(struct frame));
/*fr=list_entry(e1,struct frame,elem);*/
/* bool 
temp=pagedir_is_accessed((fr->page->thread)->pagedir,fr->page->addr);
  if(temp)
   {        
pagedir_set_accessed((fr->page->thread)->pagedir,fr->page->addr,false);*/  
     e1=list_next(e1);
        if(e1==list_end(&frametable))
          {
          e1=list_begin(&frametable);
          }    
     continue;
    }
struct frame *fr;
  if(fr->page->iswrite)
     {/* fr->page->mode=swap_out(*/
      }
  else
     {
     }
    fr->page=page;  

   lock_release(&framelock);
   return fr;
      

}


void
frame_lock (struct page *p) 
{
  
  struct frame *f=p->frame;
lock_acquire(&f->lock);
}


/* Releases frame F for use by another page.
   F must be locked for use by the current process.
   Any data in F is lost. */

void
frame_free (struct frame *f)
{

f->page=NULL;
lock_release(&f->lock);
}


/* Unlocks frame F, allowing it to be evicted.
   F must be locked for use by the current process. */

void
frame_unlock (struct frame *f) 
{
lock_release(&f->lock);

}

