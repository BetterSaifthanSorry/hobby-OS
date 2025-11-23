#ifndef NET_H
#define NET_H
#define QUEUE_SIZE 32*1024
#define ARP_PACKET_SIZE 28
#include "stdint.h"

// gloabal values for networking
extern uint8_t mac_address[6];
extern uint16_t hardware_type;
extern uint8_t idx;
extern uint16_t protocol_type;
extern uint8_t protocol_address[4];
extern uint8_t packet_queue[QUEUE_SIZE];
extern uint16_t queue_read_offset;
extern void memcpy(char* src, char* dest, uint16_t size);
extern void recieve_packet(uint16_t length);
extern uint16_t to_network16(uint16_t x);
void handle_arp_request(uint8_t*);
void create_packet(uint8_t* dst_mac, char* payload, uint32_t payload_length, uint16_t ethertype);
extern void create_ip_packet(uint8_t* src_ip, uint8_t* dst_ip, char* payload, uint16_t payload_size);
extern void recieve_udp_packet(uint8_t* packet);
void recieve_ip_packet(uint8_t* packet);
extern uint16_t from_network16(uint16_t x);
#define ARP_ETHERTYPE 0x0806
#define IPV4_ETHERTYPE 0x0800
#define ETHERNET_HEADER_SIZE 4
extern char data[512];
#endif