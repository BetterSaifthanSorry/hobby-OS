# === CONFIG ===
CC=gcc
LD=ld
AS=nasm

CFLAGS=-m32 -ffreestanding -fno-pic -fno-pie -fno-stack-protector
LDFLAGS=-m elf_i386 -T kernel.ld

ISO_DIR=isodir
KERNEL=kernel-0
ELF=$(KERNEL)
ISO=os.iso

# === FILES ===
SRC_C=kernel.c
SRC_ASM=boot.asm
OBJ=kernel.o boot.o gdt.o idt.o isr.o malloc.o vmem.o switch.o user.o lib.o syscall.o pci.o ethernet.o arp.o ip.o udp.o

# === RULES ===
all: $(ISO)

$(ISO): $(ELF)
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(ELF) $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(ISO) $(ISO_DIR)

$(ELF): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

boot.o: boot.asm
	$(AS) -f elf32 $< -o $@

gdt.o : gdt.c
	$(CC) $(CFLAGS) -c $< -o $@

isr.o : isr.c
	$(CC) $(CFLAGS) -c $< -o $@

idt.o : idt.c
	$(CC) $(CFLAGS) -c $< -o $@

malloc.o : malloc.c
	$(CC) $(CFLAGS) -c $< -o $@

vmem.o : vmem.c
	$(CC) $(CFLAGS) -c $< -o $@

switch.o : switch.c
	$(CC) $(CFLAGS) -c $< -o $@

user.o : user.c
	$(CC) $(CFLAGS) -c $< -o $@

lib.o : lib.c
	$(CC) $(CFLAGS) -c $< -o $@

syscall.o : syscall.c
	$(CC) $(CFLAGS) -c $< -o $@

pci.o : pci.c
	$(CC) $(CFLAGS) -c $< -o $@	

ethernet.o : ethernet.c
	$(CC) $(CFLAGS) -c $< -o $@	

arp.o : arp.c
	$(CC) $(CFLAGS) -c $< -o $@	

ip.o : ip.c
	$(CC) $(CFLAGS) -c $< -o $@	

udp.o : udp.c
	$(CC) $(CFLAGS) -c $< -o $@	

clean:
	rm -f *.o $(ELF) $(ISO)
	rm -rf $(ISO_DIR)/boot
