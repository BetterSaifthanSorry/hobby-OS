#ifndef PCI_H
#define PCI_H
#include<stdint.h>
#include "net.h"
#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC
#define CONFIG_1_OFFSET 0x52
#define COMMAND_REGISTER_OFFSET 0x37
#define TX_STATUS_0 0x10
#define TX_STATUS_1 0x14
#define TX_STATUS_2 0x18
#define TX_STATUS_3 0x1C
#define TX_START_0 0x20
#define TX_START_1 0x24
#define TX_START_2 0x28
#define TX_START_3 0x2C
#define ISR_OFFSET 0x3E
#define IMR_OFFSET 0x3C
#define RB_START_OFFSET 0x30
#define RCR_OFFSET 0x44
#define TRASMIT_CONFIG_OFFSET 0x40
#define ROK 1
#define TOK (1 << 2)
#define CURRENT_BUFFER_ADDRESS_OFFSET 0x3A
#define CAPR_OFFSET 0x38
#define RBLEN (64*1024)
#define HEADER_SIZE 4
#define RX_READ_POINTER_MASK ~0b11 
void init_rtl8139(void);
void rtl8139_handler(void);
void write(uint8_t*, uint16_t);
void read(void);
uint16_t strlen(char*);
#endif