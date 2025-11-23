#include "net.h"
#include "stdint.h"
#include "stdbool.h"

#define UDP_HEADER_SIZE 8

typedef struct {
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t length;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) udp_packet;

void create_udp_packet(uint16_t src_port, uint16_t dst_port, char* payload, uint16_t payload_size);
bool strcmp(char* a, char* b);
