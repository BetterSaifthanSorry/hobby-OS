#include<stdint.h>

typedef struct {
    uint16_t limit_16; // first 16 bits of 20 bit limit
    uint16_t base_16; // first 16 bits of 20 bit base address
    uint8_t base_16_24; // next 8 bits
    uint8_t access_bits;
    uint8_t flags; //also the remaining four bits of limit
    uint8_t base_24_32; // last 8 bits
} __attribute__((packed)) gdtEntry;

typedef struct {
    gdtEntry nullDescriptor;
    gdtEntry codeDescriptor;
    gdtEntry dataDescriptor;
    gdtEntry codeDescriptor_U;
    gdtEntry dataDescriptor_U;
    gdtEntry tssDescriptor;
}__attribute__((packed)) gdtTable;

/*
    hard to find spec 
    most of these fields will not be used
*/
typedef struct { // Intel manual for software devs page 3260
    uint32_t prev_tss; 
    uint32_t esp0; // stack pointer for ring 0
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtss;
    uint32_t iomap_base;
    uint32_t ssp;
} __attribute__((packed)) tss;

typedef struct{
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) gdtPtr;

extern gdtTable gdt;
extern gdtPtr ptr;

gdtEntry makeEntry(uint32_t base, uint32_t limit, uint8_t type);
void makeGDT();

