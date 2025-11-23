#include "lib.h"
#include "stdint.h"
#include "pci.h"
#include "net.h"
#define MAC_SIZE 6
#define ETHERTYPE_SIZE 2

typedef struct{
    uint8_t destination_mac[6];
    uint8_t source_mac[6];
    uint16_t ethertype;
    uint8_t payload[]; // flexible array memeber
} __attribute__((packed)) ethernet_frame;

void create_packet(uint8_t* dst_mac, char* payload, uint32_t payload_length, uint16_t ethertype);