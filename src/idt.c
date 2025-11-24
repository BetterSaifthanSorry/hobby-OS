#include "idt.h"

idtEntry idtTable[256];
idtPtr idtptr;

extern void keyboard_handler();
extern void syscall_handler();
extern void nic_handler();
void
makeIDT(void)
{
    uint32_t handler;
    handler = (uint32_t)keyboard_handler;
    idtTable[0x21].offset_1 = handler & 0xFFFF;
    idtTable[0x21].selector = 0x8;
    idtTable[0x21].zero = 0;
    idtTable[0x21].type_attributes = 0x8E;
    idtTable[0x21].offset_2 = (handler & 0xFFFF0000) >> 16;

    handler = (uint32_t)syscall_handler;
    idtTable[0x80].offset_1 = handler & 0xFFFF;
    idtTable[0x80].selector = 0x8;
    idtTable[0x80].zero = 0;
    idtTable[0x80].type_attributes = 0xEE;
    idtTable[0x80].offset_2 = (handler & 0xFFFF0000) >> 16;

    handler = (uint32_t)nic_handler;
    idtTable[0x2B].offset_1 = handler & 0xFFFF;
    idtTable[0x2B].selector = 0x8;
    idtTable[0x2B].zero = 0;
    idtTable[0x2B].type_attributes = 0x8E;
    idtTable[0x2B].offset_2 = (handler & 0xFFFF0000) >> 16;

    //ICW1
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

    //ICW2
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	//ICW3
	write_port(0x21 , 0x00);  
	write_port(0xA1 , 0x00);  

	//ICW4
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);

	//turn off interrupts for now
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

    idtptr.size = (uint16_t)(sizeof(idtTable) - 1);
    idtptr.base = (uint32_t)idtTable;

    asm volatile("lidt %0": :"m" (idtptr));
    asm volatile("sti");

    write_port(0x21 , 0xf9); // unmask line 1: keyboard line 2: slave PIC
    write_port(0xA1 , 0xf7);    //unmask line 11 : nic
}
