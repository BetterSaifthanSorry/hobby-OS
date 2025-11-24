#include "pci.h"
#include "lib.h"
#include "net.h"

uint32_t *mem_base;
uint8_t mac_address[6];
uint16_t hardware_type;

// FIXME: write malloc to allocate emmeory ont he heap so that the buffer doesnt have to be static
static uint8_t recieve_buffer[67088];
// packets are copied over to this queue from the recieve buffer for further processing
uint8_t packet_queue[QUEUE_SIZE]; 
static uint32_t buffer_read_offset = 0x0;
uint8_t trasmit_idx = 0;
uint16_t queue_read_offset = 0x0;

uint8_t TX_STATUS[4] = {TX_STATUS_0, TX_STATUS_1, TX_STATUS_2, TX_STATUS_3};
uint8_t TX_START[4] = {TX_START_0, TX_START_1, TX_START_2, TX_START_3};

uint8_t protocol_address[4] = {127, 0, 0, 1};

// this is going to be a bit strange. but for now in this init im just trying to detect the nic not any other devices.
//  using pci info in the qemu monitor, i can see that rtl8139 is at bus 0, device 3, function 0
//  TODO: create a sepearte rtl8139 file
//  TODO: Implement the linux PCI Detection with struct dev and dynamic driver installation based on class code
// TODO: disable interuppts when manipualting state

uint32_t
readConfigHeaderDword(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset)
{
    uint32_t address = (uint32_t)(0x80000000 | bus << 16 | device << 11 | function << 8 | (offset & 0xFC));
    write_port_32(CONFIG_ADDRESS, address);
    uint32_t dword = read_port_32(CONFIG_DATA);
    return dword;
}

uint16_t
readConfigHeaderWord(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset)
{
    uint32_t address = (uint32_t)(0x80000000 | bus << 16 | device << 11 | function << 8 | (offset & 0xFC));
    write_port_32(CONFIG_ADDRESS, address);
    uint16_t word = (uint16_t)((read_port_32(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    ;
    return word;
}

void writeConfigHeaderReg(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset, uint32_t data)
{
    uint32_t address = (uint32_t)(0x80000000 | bus << 16 | device << 11 | function << 8 | (offset & 0xFC));
    write_port_32(CONFIG_ADDRESS, address);
    write_port_32(CONFIG_DATA, data);
    return;
}

uint16_t
strlen(char *str)
{
    uint16_t len = 0;
    while (*(str+len++) != '\0'){}
    return len;
}

void init_rtl8139(void)
{
    // printf("Device ID: %x\n", readConfigHeaderWord(0, 3, 0, 0x2));
    uint16_t device_id = readConfigHeaderWord(0, 3, 0, 0x2);
    if (device_id == 0x8139) hardware_type=0x0100; //Ethernet Hardware
    /*
    TODO:
    Enable bus mastering
    */
    mem_base = readConfigHeaderDword(0, 3, 0, 0x14) & ~0b1111;
    // Enabling bus mastering osdev/rtl8139
    // FIX: 16 bit writes
    uint32_t command_reg = readConfigHeaderDword(0, 3, 0, 0x4);
    // printf("Before write: %x", command_reg);
    command_reg = command_reg | 0b100;
    writeConfigHeaderReg(0, 3, 0, 0x4, command_reg);
    // printf("After write: %x", readConfigHeaderDword(0, 3, 0, 0x4));

    

    // get the interupt line
    uint8_t interrupt_line = readConfigHeaderWord(0, 3, 0, 0x3C) & 0xFFFF;
    // printf("Interrupt Line: %d\n", interrupt_line);

    ((uint8_t *)mem_base)[CONFIG_1_OFFSET] = 0x0;

    uint8_t old_val = ((uint8_t *)mem_base)[COMMAND_REGISTER_OFFSET];
    // printf("Before Write: %b", old_val);
    // sending software reset
    ((uint8_t *)mem_base)[COMMAND_REGISTER_OFFSET] = 0b00010000;
    while ((((uint8_t *)mem_base)[COMMAND_REGISTER_OFFSET] & 0x10) == 1)
    {
        printf("Polling...\n");
    }
    uint8_t new_val = ((uint8_t *)mem_base)[COMMAND_REGISTER_OFFSET];
    // printf("After Write: %b", new_val);

    printf("Mac Address: ");
    read_mac_address();
    for (int i = 0; i < 5; i++) 
        printf("%x:", mac_address[i]);
    printf("%x\n", mac_address[5]);
    

    // FIXME: write malloc to allocate emmeory ont he heap so that the buffer doesnt have to be static
    char *dummy = "this is a string that has to be atleast 64 bytes characters.";
    // printf("Strlen 1: %d", strlen(dummy));
    char *dummy2 = "this is a string that has to be atleast 64 bytes character cunt cuntcuntcuntcuntcuntcunt.";
    // printf("Strlen 2: %d", strlen(dummy2));

    // Enabling Tx and Rx
    ((uint8_t *)mem_base)[COMMAND_REGISTER_OFFSET] = 0x0C;

    // enabling interrupts
    // printf("Default Value IMR: %x", ((uint16_t*)mem_base)[IMR_OFFSET/2]);
    ((uint16_t *)mem_base)[IMR_OFFSET / 2] = 0x0005;
    // printf("New Value IMR: %x", ((uint16_t *)mem_base)[IMR_OFFSET / 2]);

    // init recieve buffer
    mem_base[RB_START_OFFSET / 4] = recieve_buffer;

    // init RCR
    mem_base[RCR_OFFSET / 4] = (0x0000000f | (1 << 7) | (1 << 11) | (1 << 12) | (1 << 5));
    // printf("RCR: %x\n", mem_base[RCR_OFFSET / 4]);

    // // Setting trasmit and configuration registers
    // mem_base[0x40/4] = 0x03000700;
    // mem_base[0x44] = 0x0000070a;

    // enable loopback mode
    // uint32_t old_val_config = mem_base[TRASMIT_CONFIG_OFFSET / 4];
    // old_val_config |= (1 << 17);
    // old_val_config |= (1 << 18);
    // mem_base[TRASMIT_CONFIG_OFFSET / 4] = old_val_config;

    // for(int i = 0; i< 10; i++)
    // {
    // write(dummy);
    // read();

    // write(dummy2);
    // read();

    // write(dummy2);
    // read();

    // write(dummy2);
    // read();
    // }
    // for (uint8_t* i = recieve_buffer; i < buffer_read_offset; i++) 
    // {
    //     printf("%c, ", *i);
    // }
}

void 
turn_off_loopback_mode(void){
    uint32_t old_val_config = mem_base[TRASMIT_CONFIG_OFFSET / 4];
    old_val_config &= (0 << 17);
    old_val_config &= (0 << 18);
    mem_base[TRASMIT_CONFIG_OFFSET / 4] = old_val_config;
}

void rtl8139_handler(void)
{
    uint16_t isr = ((uint16_t *)mem_base)[ISR_OFFSET / 2];
    // ((uint16_t*)mem_base)[ISR_OFFSET/2] = 0x05;
    if (isr & TOK){
        ((uint16_t *)mem_base)[ISR_OFFSET / 2] = (uint16_t)TOK;
        // printf("transmission complete\n");
    }
    if (isr & ROK){
        ((uint16_t *)mem_base)[ISR_OFFSET / 2] = (uint16_t)ROK;
        // printf("reception complete\n Turning off loopback\n");
        read();
    }
    write_port(0x20, 0x20);  
    write_port(0xA0, 0x20); 
}

void increment_capr(void)
{
    uint16_t packet_length = *((uint16_t*)(recieve_buffer + buffer_read_offset) + 1);
    // printf("Packet Length in Increment: %d", packet_length);
    buffer_read_offset = (buffer_read_offset + packet_length + HEADER_SIZE + 3) % RBLEN;
    buffer_read_offset = buffer_read_offset & ~0b11;
    ((uint16_t*)mem_base)[CAPR_OFFSET/2] = buffer_read_offset - 0x10;
}

void print_packet()
{
    uint16_t packet_length = *((uint16_t*)(recieve_buffer + buffer_read_offset) + 1) - ETHERNET_HEADER_SIZE;
    printf("Recieved packet: ");
    for (int i = 0; i < packet_length - 4; i++)
    {
        printf("%c", (recieve_buffer + buffer_read_offset + 4 + ETHERNET_HEADER_SIZE)[i]);
    }
    printf("\n");
}

void write(uint8_t *packet, uint16_t length)
{
    // printf("Size when writing to wire: %x\n", length);
    // writing the address of dummy data to ts-0
    mem_base[TX_START[trasmit_idx] / 4] = (uint32_t)packet;
    uint32_t default_val = mem_base[TX_STATUS[trasmit_idx] / 4];
    default_val &= ((uint32_t)0 << 13);
    default_val |= (length & 0xffff);
    mem_base[TX_STATUS[trasmit_idx] / 4] = default_val;
    trasmit_idx = (trasmit_idx + 1) % 4;
}

void read(void)
{
    // printf("Rx packet Header: %x\n", *((uint16_t*)recieve_buffer + buffer_read_offset));
    uint16_t length = *((uint16_t*)(recieve_buffer + buffer_read_offset) + 1);
    // printf("Rx packet length: %x\n", length);
    memcpy(recieve_buffer+buffer_read_offset+4, packet_queue+queue_read_offset, length);
    recieve_packet(length);
    // uint8_t packet[0x40];
    // memcpy(recieve_buffer+buffer_read_offset+4, packet, length);
    // write(packet, length);
    // print_packet();
    increment_capr();
}

// FIXME: Read from eeprom with bit banging instead of mac regsiters
void read_mac_address(void) { for(int i = 0; i < 6; i++) mac_address[i] = ((uint8_t*)mem_base)[i]; } 