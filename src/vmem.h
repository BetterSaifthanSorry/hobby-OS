#ifndef VMEM_H
#define VMEM_H
#include <stdint.h>
#include <stdbool.h>
#include "malloc.h"
#define PG_SIZE 4096
#define NO_PAGES 1024 // No of pages

typedef uint32_t va; // Virtual address
typedef uint32_t pa; // Physical address 
typedef uint32_t pde; // Page directory entry 
typedef uint32_t pte; // Page table entry 

#define DIR_INDEX(virtual_address) ((virtual_address >> 22) & 0x3FF)
#define TABLE_INDEX(virtual_address) ((virtual_address >> 12) & 0x3FF)
#define PDE_FLAGS 0x3
#define PDE_PRESENT 0x1
#define SET_ADDRESS(entry, address) *entry = ((uint32_t)*entry | ((uint32_t)address & 0xFFFFF000))
#define GET_ADDRESS(entry) (*entry & 0xFFFFF000)

typedef struct {
   pte table[NO_PAGES];
} __attribute__((packed)) pagetable;

typedef struct {
    pde directory[NO_PAGES];
} __attribute__((packed)) pagedir;

bool map(pagedir*, va virtual_address, pa physical_address, int cache);
void create_kpgtbl(void);
void create_upgtbl(void);
#endif