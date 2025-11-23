#include "ethernet.h"

uint16_t protocol_type = 0x0008; //IPv4

uint8_t char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0; 
}

void
create_packet(uint8_t* dst_mac, char* payload, uint32_t payload_length, uint16_t ethertype){
    payload_length = (payload_length >= 46)? payload_length : 46;
    uint32_t frame_size = payload_length + 2*MAC_SIZE + ETHERTYPE_SIZE;
    uint8_t frame_array[frame_size];
    for (uint16_t i=0; i < frame_size; i++) {frame_array[i] = 0x0;}
    ethernet_frame* frame = (ethernet_frame*)frame_array;
    memcpy(mac_address, frame->source_mac, 6);
    memcpy(dst_mac, frame->destination_mac, 6);
    frame->ethertype=to_network16(ethertype);
    memcpy(payload, frame->payload, payload_length);
    // printf("Size when passing from ethernet layer to wire: %x\n", sizeof(frame));
    write((uint8_t*)frame, frame_size);
}

void 
recieve_packet(uint16_t length){
    // printf("Recieved Ethernet packet\n");
    uint8_t frame_array[length];
    memcpy(packet_queue+queue_read_offset, frame_array, length);
    ethernet_frame* frame = (ethernet_frame*)frame_array;
    // printf("Ethertype: %x\n", frame->ethertype);
    if (frame->ethertype == to_network16(ARP_ETHERTYPE))
        handle_arp_request((uint8_t*)(frame->payload));
    else
        recieve_ip_packet(frame->payload);
}