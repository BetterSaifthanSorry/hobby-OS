#include<stdint.h>
#include<stdbool.h>
#include "ethernet.h"

#define ARP_CACHE_SIZE 128
//repalce static declaration with malloc call 

typedef struct{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_length;
    uint8_t protocol_length;
    uint16_t opcode;
    uint8_t sender_hardware_addr[6];
    uint8_t sender_protocol_address[4];
    uint8_t target_hardware_addr[6];
    uint8_t target_protocol_address[4];
}__attribute__((packed)) arp_packet;

// format taken from linux arp table, command "arp" in wsl displays the arp cache
typedef struct {
    uint8_t protocol_address[4];
    uint16_t hardware_type;
    uint8_t hardware_address[6];
}__attribute__((packed)) arp_cache_entry; 
// flag and interface field not needed (for now)

void create_arp_request_packet(uint8_t* src_mac);
arp_cache_entry* lookup_arp_cache(uint8_t* ip_addr);
static arp_cache_entry arp_cache[ARP_CACHE_SIZE];