#include "vmem.h"
pagedir* k_pagedir; //kernel page directory

bool
map(pagedir* dir,va virtual_address, pa physical_address, int cache)
{
    pde* entry = &dir->directory[DIR_INDEX(virtual_address)];
    if ((*entry & PDE_PRESENT) == 0)
    {
        char* page = alloc();
        if (!page) return false;
        SET_ADDRESS(entry, page);
        *entry |= PDE_FLAGS;
        *entry |= PDE_PRESENT;
        *entry |= 0x4;
        if (cache) *entry |= 0b10000;
    }
    pagetable* table = (pagetable*)(GET_ADDRESS(entry));
    pte* pte_entry = &(table->table[TABLE_INDEX(virtual_address)]);
    SET_ADDRESS(pte_entry, physical_address);
    *pte_entry |= PDE_FLAGS;
    *pte_entry |= 0x4;
    if (cache) *entry |= 0b10000;
    return true;
}

void 
create_kpgtbl(void)
{
    //first we'll create the page directory
    k_pagedir = (pagedir*)alloc();
    /*  map two pages
        => one for all the kernel code 
        => one for the first 4MiB region */

    pa physical_address;
    va virtual_address;

    // 0-4MiB region
    for (int i = 0, physical_address = 0, virtual_address = 0; i < NO_PAGES; i++, physical_address+=PGSIZE, virtual_address+=PGSIZE)
        map(k_pagedir, virtual_address, physical_address, 0);
    
    // 0xFEB91000 - Mem base for rtl8139
    for (int i = 0, physical_address = 0xFEB91000, virtual_address = 0xFEB91000; i < NO_PAGES; i++, physical_address+=PGSIZE, virtual_address+=PGSIZE)
        map(k_pagedir, virtual_address, physical_address, 1);
    
    
    uint32_t pd_phys = (uint32_t)k_pagedir;
    asm volatile (
        "mov %0, %%cr3\n"      // Load page directory base into CR3
        "mov %%cr0, %%eax\n"   // Move CR0 into EAX
        "or $0x80000001, %%eax\n" // Set PG (bit 31) and PE (bit 0)
        "mov %%eax, %%cr0\n"   // Write back to CR0
        : : "r"(pd_phys) : "eax"
    );
    return;
}

void 
create_upgtbl(void)
{
    //first we'll create the page directory
    pagedir* u_pagedir = (pagedir*)alloc();
    /*  map two pages
        => one for all the kernel code 
        => one for the first 4MiB region */

    pa physical_address;
    va virtual_address;

    // 0-4MiB region
    for (int i = 0, physical_address = 0, virtual_address = 0; i < 4*NO_PAGES; i++, physical_address+=PGSIZE, virtual_address+=PGSIZE)
        map(u_pagedir, virtual_address, physical_address, 0);
    
    // 0xFEB91000 - Mem base for rtl8139
    for (int i = 0, physical_address = 0xFEB91000, virtual_address = 0xFEB91000; i < NO_PAGES; i++, physical_address+=PGSIZE, virtual_address+=PGSIZE)
        map(u_pagedir, virtual_address, physical_address, 1);
    
    uint32_t pd_phys = (uint32_t)u_pagedir;
    asm volatile (
        "mov %0, %%cr3\n"      
        "mov %%cr0, %%eax\n"   
        "or $0x80000001, %%eax\n" 
        "mov %%eax, %%cr0\n"   
        : : "r"(pd_phys) : "eax"
    );
    return;
}