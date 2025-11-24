#include"gdt.h"
#include"idt.h"
#include"malloc.h"
#include"vmem.h"
#include "switch.h"
#include "lib.h"
#include "pci.h"
#include "net.h"
#include "ethernet.h"
#include "ip.h"
#include "udp.h"
// #include "arp.h"

void kmain(void)
{
	makeGDT();
	makeIDT();
	makeFreeList();
	create_kpgtbl();
	init_rtl8139();
	// create_arp_request_packet("12:23:45:65:54:32");
	// read();
	// uint8_t src_ip = {127, 126, 10, 1};
	// uint8_t dest_ip = {127, 126, 10, 1};
	// create_ip_packet(src_ip, dest_ip, "My name is slim shady!", 22);
	// read();
	// create_udp_packet(101, 102, "Hello this is rajneesh speaking how amy i help you?", 52);
	// printf("Index: %d\n", idx);
	printf("In kernel space\n");
	// write("Hello this is rajneesh speaking!", 33);
	// create_udp_packet(101, 102, "Hello this is rajneesh speaking.", 33);
	// write("Hello this is rajneesh!", 33);
	for(int i=0; data[i] != '\0'; i++)
		printf("%c", data[i]);
	printf("\n");
	// if(strcmp(data, "switch") == true)
	// {
		printf("Switching to user space!\n");
		switch_space();
	// }
}