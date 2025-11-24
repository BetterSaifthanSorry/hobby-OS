# x86 Hobby OS

A 32-bit operating system that boots via the GRUB bootloader, sets up its GDT, IDT and, TSS, enables paging with simple identity mapping, manually probes the PCI bus to detect the RTL8139, sets up the transmission and reception buffers for the NIC, registers the required interrupt handlers, initializes the data structures for a minimal Ethernet/IP/UDP stack, and finally demonstrates a jump form ring 0 to ring 3. It’s written as an educational project, but it does demonstrates a decent amount of real kernel design patterns.

## Highlights

- Custom GDT with ring0/ring3 segments, TSS setup, PIC remap, and IDT entries for keyboard, NIC, and syscall interrupts.
- Paging.
- RTL8139 initialization, interrupt handling, RX ring buffering, and TX path through a basic Ethernet/IP/UDP pipeline (with ARP support).
- User-mode trampoline (`switch_space`) that builds a trapframe and executes a demo `int 0x80` syscall from ring 3.

## Repository Layout

- `src/boot.asm` – Multiboot header, bootstrap, ISR stubs.
- `src/*.c` – Kernel subsystems (memory management, drivers, networking, user demo, etc.).
- `src/isodir` – GRUB-ready tree used by `grub-mkrescue`.
- `config/` – Runtime artifacts (disk image, logs) when using Bochs/QEMU.

## Building

Prerequisites (Linux/macOS or WSL recommended):

- `gcc` (32-bit capable) and `ld`
- `nasm`
- `grub-mkrescue`
- `qemu-system-i386`

### Compile and Package

```bash
cd src
make
```

This produces `kernel-0` plus a bootable `os.iso` under `src/`.

### Run in QEMU

```bash
cd src
make qemu
```

## Next Steps
Next I plan to add a shell, flesh out the syscall interface even more, add proper process management.
Ultimately I would love to add a TCP layer over the existing stack, and maybe also write a USB driver. These goals are perhaps over-ambitious.



## Network Stack Demo 
To test the network stack, run the kernel with make qemu which will enable port forwarding from the host. In a seperate terminal, run netcat using the command `nc -u 127.0.0.1 6001`, which will open a UDP connection from the host machine to the OS. Type anything into the terminal and press Enter, the packet will then show up inside the kernel.
