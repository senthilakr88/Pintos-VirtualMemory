#ifndef VM_FRAME_H
#define VM_FRAME_H

#include <stdbool.h>
#include "threads/synch.h"


struct list frametable;
/* A physical frame. */
struct frame 
{
    void *base;                 /* Kernel virtual base address. */
    struct page *page;          /* Mapped process page, if any. */
    struct lock lock;
    struct list_elem elem;
   /* struct thread *thread;*/
};

void frame_init (void);

struct frame *frame_alloc_and_lock (struct page *);
void frame_lock (struct page *);

void frame_free (struct frame *);
void frame_unlock (struct frame *);

#endif /* vm/frame.h */
