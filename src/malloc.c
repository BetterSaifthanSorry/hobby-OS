#include"malloc.h"
extern char end[];

freeNode* head;

void 
makeFreeList(void) {
    head = 0;
    for(uint32_t addr = end; addr < MAXPA; addr+=PGSIZE)
    {
        freeNode* curr = (freeNode*)(addr);
        curr->next=head;
        head=curr;
    }
}

// this is a bit different from the C malloc since it gives out one page.
char*
alloc(void)
{
    if (head)
    {
        char* page = head;
        head = head->next;
        return page;
    }
} 