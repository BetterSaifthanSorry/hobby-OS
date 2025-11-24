#include "key_mappings.h"
int current_loc = 20;

void
keyboard_isr(void){
    unsigned char status;
	char keycode;

	write_port(0x20, 0x20); // end-of-interrupt

	char *vidptr = (char*)0xb8000; 	

	status = read_port(0x64);
	if (status & 0x01) {
		keycode = read_port(0x60);
		if(keycode < 0)
			return;
		vidptr[current_loc++] = keyboard_map[keycode];
		vidptr[current_loc++] = 0x07;	
	}
}

