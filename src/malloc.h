#ifndef MALLOC_H
#define MALLOC_H
#include <stdint.h>

#define MAXPA 0x200000 //Assuming a 256-MB machine 
#define PGSIZE 0x1000

//add to a free list

typedef struct freeNode{
    struct freeNode* next; 
} freeNode;

void makeFreeList(void);
#endif