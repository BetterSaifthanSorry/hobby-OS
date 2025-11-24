#include<stdint.h>
#include "net.h"

#define MAY_FRAGMENT_FLAG 0
#define LAST_FRAGMENT_FLAG 0 
#define MORE_FRAGMENT_FLAG 1 << 2
#define DO_NOT_FRAGMENT_FLAG 1 << 1
#define MIN_IP_HEADER_SIZE 5
#define UDP_PROTOCOL_NUMBER 0x11

static uint16_t id = 0;

typedef struct {
    uint8_t protocol_and_length; // header length
    uint8_t ToS; // mostly obsolete now(?)
    uint16_t total_length; // total length
    uint16_t id;
    uint16_t flags_and_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
    uint8_t data[];
} __attribute__((packed)) ip_header;


// TODO: 
// typedef struct{

// } routing_table_entry;

void create_ip_packet(uint8_t* src_ip, uint8_t* dst_ip, char* payload, uint16_t payload_size);
void set_ip_header_flags(uint8_t flags, ip_header* header);
void set_ip_header_protocol(ip_header* header, uint8_t protocol);
void set_ip_header_length(ip_header* header, uint8_t length);