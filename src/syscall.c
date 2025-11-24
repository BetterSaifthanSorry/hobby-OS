#include "lib.h"
#include "syscall.h"
#include <stdint.h>

int test_sys_1(void)
{
    printf("Back in kernel space (1)\n");
    return 1;
}

int test_sys_2(void)
{
    printf("Back in kernel space (2)\n");
    return 1;
}

int (*syscalls[])(void)  = {
    [SYS_TEST0] = test_sys_1, 
    [SYS_TEST1] = test_sys_2, 
};

void 
syshandler(uint32_t syscall_number){
    syscalls[syscall_number]();
}