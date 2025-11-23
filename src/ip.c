#include "ip.h"

uint16_t to_network16(uint16_t x) {
    const uint16_t one = 1;
    if (*(const uint8_t *)&one == 1) {
        return (uint16_t)((x << 8) | (x >> 8));
    }
    return x;
}

// TODO: add support for options in ip header
void create_ip_packet(uint8_t* src_ip, uint8_t* dst_ip, char* payload, uint16_t payload_size){
    uint8_t ip_header_array[MIN_IP_HEADER_SIZE * 4 + payload_size];
    ip_header* ip_packet = (ip_header*)ip_header_array;
    for (uint16_t i = 0; i < MIN_IP_HEADER_SIZE*4 + payload_size; i++) {ip_header_array[i] = 0x0;}
    set_ip_header_protocol(ip_packet, 0x40); // IPv4
    set_ip_header_length(ip_packet, MIN_IP_HEADER_SIZE);
    // ignoring type of service
    ip_packet->total_length = to_network16(MIN_IP_HEADER_SIZE * 4 + payload_size); 
    ip_packet->id = id;
    ip_packet->ttl = 64;
    ip_packet->protocol=0x11; 
    ip_packet->header_checksum = 0x00;
    // ip_packet->options=0;
    memcpy(src_ip, ip_packet->src_ip, 4);
    memcpy(dst_ip, ip_packet->dst_ip, 4);
    memcpy(payload, ip_packet->data, payload_size);
    uint8_t dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    create_packet(dst_mac, ip_packet, MIN_IP_HEADER_SIZE * 4 + payload_size, IPV4_ETHERTYPE);
}

void
recieve_ip_packet(uint8_t* packet){
    // printf("Recieved IP packet\n");
    ip_header* ip = (ip_header*)packet;
    // if (ip->protocol == UDP_PROTOCOL_NUMBER)
        recieve_udp_packet(ip->data);
}

void 
set_ip_header_protocol(ip_header* header, uint8_t protocol){
    uint8_t protocol_and_length = header->protocol_and_length;
    protocol_and_length |= (protocol & 0xf0);
    header->protocol_and_length = protocol_and_length;
}

void 
set_ip_header_length(ip_header* header, uint8_t length){
    uint8_t protocol_and_length = header->protocol_and_length;
    protocol_and_length |= (length & 0x0f);
    header->protocol_and_length = protocol_and_length;
}

void 
set_ip_header_flags(uint8_t flags, ip_header* header){
    uint8_t flags_and_offset = header->flags_and_offset;
    flags_and_offset |= flags;
    header->flags_and_offset = flags_and_offset; 
}


