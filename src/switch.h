/*
    In this file I want to lay down the groundwork for switching from the kernel to "userspace"
    I'll add two more segments to the GDT with ring3 privelleges
    Biuld the paging table for the user process with the kernel mapping intact and some enw mapping for some entry va to a pa storing a small user program
    biuld a synthetic trapframe and call iret
*/
#include "vmem.h"

void create_trapframe(void); // this function will create a trapframe and call iret 
void switch_space(void);