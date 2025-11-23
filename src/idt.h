#include <stdint.h>

typedef struct{
   uint16_t offset_1;        // lower offset bits
   uint16_t selector;        
   uint8_t  zero;            // no purpose cruft
   uint8_t  type_attributes; // gate type
   uint16_t offset_2;        // higher offset bits
}__attribute__((packed)) idtEntry;

typedef struct {
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) idtPtr;

void makeIDT(void);