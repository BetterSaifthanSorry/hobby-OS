#include "lib.h"
void user_main(void)
{
    printf("In user space!\n");
        asm volatile(
        ".intel_syntax noprefix\n\t"
        "mov eax, 0\n\t"
        "int 0x80\n\t"
        ".att_syntax prefix\n\t"
        : /* no outputs */
        : /* no inputs */
        : "eax"
    );
    printf("Back in user space!\n");
    for (;;)
    {
    }
}