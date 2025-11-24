#include "udp.h"
char data[512];

bool strcmp(char* a, char* b) {
    while(*a++ == *b++){
        if (!(*(--a))) return true;
    }
    return false;
}

uint16_t from_network16(uint16_t x) {
    return (uint16_t)((x >> 8) | (x << 8));
}

void create_udp_packet(uint16_t src_port, uint16_t dst_port, char* payload, uint16_t payload_size){
    uint16_t packet_size = UDP_HEADER_SIZE + payload_size;
    uint8_t udp_packet_array[packet_size];
    udp_packet* packet = (udp_packet*)udp_packet_array;
    packet->src_port=to_network16(src_port);
    packet->dest_port=to_network16(dst_port);
    packet->length=to_network16(packet_size);
    for (uint16_t i = 0; i < payload_size; i++) {packet->data[i]=payload[i];}
    uint8_t src_ip[4] = {129, 127, 10, 1};
    uint8_t dest_ip[4] = {129, 127, 11, 2};
    create_ip_packet(src_ip, dest_ip, packet, packet_size);
}

void 
recieve_udp_packet(uint8_t* packet){
    udp_packet* udp = (udp_packet*)packet;
    uint16_t length = from_network16(udp->length) - UDP_HEADER_SIZE;
    // printf("length: %d\n", length);
    printf("Recieved Udp Packet: ");
    for(uint16_t i = 0; i < length; i++) {printf("%c", udp->data[i]);}
    printf("\n");
    memcpy(udp->data, data, length);
    data[length] = '\0'; 
}