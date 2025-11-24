#include "gdt.h"
#include<stdint.h>

__attribute__((section(".data"), aligned(8))) gdtTable gdt;   
__attribute__((section(".data"), aligned(8))) gdtPtr ptr; 
__attribute__((section(".data"), aligned(8))) tss _tss; 

gdtEntry 
makeEntry(uint32_t base, uint32_t limit, uint8_t type){
    gdtEntry entry;
    entry.limit_16 = limit & 0xFFFF;
    entry.base_16 = base & 0xFFFF;
    entry.access_bits = type;
    entry.flags = ((limit >> 16) & 0x0F) | 0xC0;
    entry.base_16_24 = (base >> 16) & 0xFF;
    entry.base_24_32 = (base >> 24) & 0xFF;
    return entry;
}


void 
makeGDT(void){
    gdt.nullDescriptor = makeEntry(0, 0, 0);
    gdt.codeDescriptor = makeEntry(0, 0xFFFFF, 0x9A);
    gdt.dataDescriptor = makeEntry(0, 0xFFFFF, 0x92);
    gdt.codeDescriptor_U = makeEntry(0, 0xFFFFF, 0xFA);
    gdt.dataDescriptor_U = makeEntry(0, 0xFFFFF, 0xF2);
    gdt.tssDescriptor = makeEntry(&_tss, 0x67, 0x89);

    // initialize the tss
    _tss.esp0 = 0x1000000;
    _tss.ss0 = 0x10;

    ptr.size = (uint16_t)(sizeof(gdt) - 1);
    ptr.base = (uint32_t)&gdt;

    asm volatile("lgdt %0": : "m" (ptr));
    asm volatile ("ltr %%ax" : : "a" (0x28));
    return;
}