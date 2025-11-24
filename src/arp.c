/*
    arp implementation based on the original rfc from 1982 (rfc 826)
*/
#include "arp.h"
#include "net.h"
uint8_t idx = 0;

static uint8_t char_to_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0; 
}

void 
memcpy(char* src, char* dest, uint16_t bytes)
{
    while(bytes){
        *dest++=*src++;
        --bytes;
    } 
}

void 
swap(char* src, char* dest)
{
    uint8_t temp = *src;
    *src=*dest;
    *dest=temp; 
}

bool ip_eq(uint8_t* ip_a, uint8_t* ip_b) {return (ip_a[0]==ip_b[0]) && (ip_a[1]==ip_b[1]) && (ip_a[2]==ip_b[2]) && (ip_a[3]==ip_b[3]);}


// for now lets hardcode in ipv4 over ethernet
void
create_arp_request_packet(uint8_t* src_mac){
    uint8_t arp_mem[sizeof(arp_packet)];
    arp_packet* packet = (arp_packet*)arp_mem;
    for(uint16_t i = 0; i < sizeof(arp_packet); i++) arp_mem[i] = 0x00;
    packet->hardware_type=0x0100;
    packet->protocol_type=0x0008;
    packet->hardware_length=0x06;
    packet->protocol_length=0x04;
    packet->opcode=0x0100;
    uint8_t idx=0;

    for(int i=0; i < 17; i++){
        uint8_t first_digit=char_to_hex(src_mac[i]), second_digit=char_to_hex(src_mac[i+1]);
        uint8_t byte = ((first_digit << 4) | (second_digit & 0xFF));
        packet->sender_hardware_addr[idx++] = byte;
    }
    for(uint8_t i=0; i < 4;i++) packet->sender_protocol_address[i] = i;
    for(uint8_t i=0; i < 4;i++) packet->target_protocol_address[i] = protocol_address[i];
    for(uint8_t i=0; i < 6;i++) packet->target_hardware_addr[i] = 0x00;
    printf("Size when passing from Arp layer to ethernet: %x\n", sizeof(arp_packet));
    uint8_t dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    create_packet(dst_mac, (char*)packet, sizeof(arp_packet), ARP_ETHERTYPE);
}

void 
handle_arp_request(uint8_t* raw_packet){
    // printf("handling arp request\n");
    arp_packet* packet = (arp_packet*)raw_packet;
    if (packet->hardware_type==hardware_type)
    {
        if(packet->protocol_type==protocol_type)
        {
            uint8_t merge = 0;
            arp_cache_entry* found_entry = lookup_arp_cache(packet->sender_protocol_address);
            if (found_entry->hardware_type!=0xffff) //found
            {
                memcpy(packet->sender_hardware_addr, found_entry->hardware_address, 6);
                merge=1;
            }   
            if(ip_eq(packet->target_protocol_address, protocol_address))
            {
                if(!merge){
                    arp_cache_entry entry;
                    memcpy(packet->sender_protocol_address, entry.protocol_address, 4);
                    memcpy(packet->sender_hardware_addr, entry.hardware_address, 6);
                    entry.hardware_type = packet->hardware_type;
                    add_arp_cache_entry(entry);
                }
                if (packet->opcode==0x0100) // is it a request
                {
                    for(int i=0; i < 6; i++)
                        packet->target_hardware_addr[i]=mac_address[i];
                    for(int i = 0; i < 4; i++)
                        swap(packet->sender_protocol_address + i, packet->target_protocol_address + i);
                    for(int j=0; j < 6; j++)
                        swap(packet->sender_hardware_addr+j, packet->target_hardware_addr+j);
                    packet->opcode=0x0200;
                    // printf("packet creation\n");
                    uint8_t dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
                    create_packet(dst_mac, (char*)packet, sizeof(arp_packet), ARP_ETHERTYPE);
                }
            }
            else
            printf("Not for me!!\n");
        }
        else
            printf("Unsupported protocol type!!\n");
    }
    else
        printf("Unsupported Harware Type!!\n");
}

void 
init_arp_cache(void)
{
    arp_cache[0].hardware_type=0xffff; //special arp entry to signify arp entry not found
}

arp_cache_entry* 
lookup_arp_cache(uint8_t* ip_addr){
    for (int i = 0; i < idx; i++)
    {
        if (ip_eq(ip_addr, arp_cache[i].protocol_address))
            return &arp_cache[i];
    }
    return &arp_cache[idx];
}

void
add_arp_cache_entry(arp_cache_entry entry){
    if (idx < ARP_CACHE_SIZE)
    {
        arp_cache_entry null_entry = arp_cache[idx];
        arp_cache[idx++] = entry;
        arp_cache[idx] = null_entry;
    }
    else    
        printf("ARP cache at max capacity!!");
}