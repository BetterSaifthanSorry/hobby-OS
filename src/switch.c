#include "switch.h"

extern void user_main(void);

void 
create_trapframe(void) {
    uint32_t user_stack = 0x7FF000;
    uint32_t user_eflags = 0x202; 
    asm volatile (
        "cli\n\t"
        "mov $0x23, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "push $0x23\n\t"
        "push %[ustack]\n\t"
        "push %[ueflags]\n\t"
        "push $0x1b\n\t"
        "push %[uentry]\n\t"
        "iret\n\t"
        :
        : [ustack] "r" (user_stack), [uentry] "r" (user_main), [ueflags] "r" (user_eflags)
        : "ax"
    );
}

void
switch_space(void){
    create_upgtbl();
    create_trapframe();
}