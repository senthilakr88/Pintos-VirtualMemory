#include "vm/page.h"
#include <stdio.h>
#include <string.h>
#include "vm/frame.h"
#include "vm/swap.h"
#include "filesys/file.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "threads/loader.h"


/* Maximum size of process stack, in bytes. */
#define STACK_MAX (1024 * 1024)
struct hash pages;
/* Destroys a page, which must be in the current process's
   page table.  Used as a callback for hash_destroy(). */
static void
destroy_page (struct hash_elem *p_, void *aux UNUSED)
{
struct page *p=hash_entry(p_,struct page,hash_elem);
  frame_free(p->frame); 

   
}

/* Destroys the current process's page table. */
void
page_exit (void) 
{

hash_destroy(thread_current()->pages,destroy_page);

}

/* Returns the page containing the given virtual ADDRESS,
   or a null pointer if no such page exists.
   Allocates stack pages as necessary. */
static struct page *
page_for_addr (const void *address) 
{
   struct page *pr;
   struct hash_elem *e1;
   // pr->addr=pg_round_round(address);
   e1=hash_find(thread_current()->pages,&pr->hash_elem);
   pr=hash_entry(e1,struct page,hash_elem);
  return pr;

}

/* Locks a frame for page P and pages it in.
   Returns true if successful, false on failure. */
/*static bool
do_page_in (struct page *p)
{


if(p->frame!=NULL)
return true;

}*/

/* Faults in the page containing FAULT_ADDR.
   Returns true if successful, false on failure. */
bool
page_in (void *fault_addr) 
{

     struct page *pr;
     pr=malloc(sizeof(struct page));
     pr=page_for_addr(fault_addr);
     if(pr!=NULL)
      return true;
    else
      return false;

}

/* Evicts page P.
   P must have a locked frame.
   Return true if successful, false on failure. */
bool
page_out (struct page *p) 
{
frame_unlock(p->frame);
free(p);
return true;

}

/* Returns true if page P's data has been accessed recently,
   false otherwise.
   P must have a frame locked into memory. */
bool
page_accessed_recently (struct page *p) 
{
bool temp;
temp=pagedir_is_accessed(p->thread->pagedir,p->addr);
return temp;
}

/* Adds a mapping for user virtual address VADDR to the page hash
   table.  Fails if VADDR is already mapped or if memory
   allocation fails. */
struct page *
page_allocate (void *vaddr, bool read_only)
{
hash_init(&pages,page_hash,page_less,NULL);
struct hash_elem * elem;
struct page *pa=malloc(sizeof(struct page));
pa->thread=thread_current();
pa->addr=pg_round_down(vaddr);
pa->read_only=read_only;
elem=hash_insert(thread_current()->pages,&pa->hash_elem);

if(elem!=NULL)
 { printf("Already the page is existing in the table");
  return NULL;}
else
{ printf("the page is inserted");
  return pa;
}
}
/* Evicts the page containing address VADDR
   and removes it from the page table. */
void
page_deallocate (void *vaddr) 
{
struct page *pr;
pr=malloc(sizeof(struct page));
pr->addr=pg_round_down(vaddr);
struct hash_elem *e1=hash_find(thread_current()->pages,&pr->hash_elem);
pr=hash_entry(e1,struct page,hash_elem);
if(pr->frame->page)
{
frame_free(pr->frame);
}
hash_delete(thread_current()->pages,&pr->hash_elem);

}






/* Returns a hash value for the page that E refers to. */
unsigned
page_hash (const struct hash_elem *e, void *aux UNUSED) 
{
const struct page *pr=hash_entry(e,struct page,hash_elem);
return ((uintptr_t)pr->addr)>>PGBITS;


}

/* Returns true if page A precedes page B. */
bool
page_less (const struct hash_elem *a_, const struct hash_elem *b_,
           void *aux UNUSED) 
{

const struct page *a=hash_entry(a_,struct page,hash_elem);
const struct page *b=hash_entry(b_,struct page,hash_elem);

return a->addr<b->addr;


}

/* Tries to lock the page containing ADDR into physical memory.
   If WILL_WRITE is true, the page must be writeable;
   otherwise it may be read-only.
   Returns true if successful, false on failure. */
bool
page_lock (const void *addr, bool will_write) 
{
struct page *p;
struct hash_elem *e1;
p->addr=pg_round_down(addr);
e1=hash_find(thread_current()->pages,&p->hash_elem);
p=hash_entry(e1,struct page,hash_elem);
if(will_write)
{}
if(p->frame)
return true;
else
return false;



}

/* Unlocks a page locked with page_lock(). */
void
page_unlock (const void *addr) 
{
struct page *pr;
struct hash_elem *e1;
pr->addr=pg_round_down(addr);
e1=hash_find(thread_current()->pages,&pr->hash_elem);
pr=hash_entry(e1,struct page,hash_elem);
frame_unlock(pr->frame);

}

