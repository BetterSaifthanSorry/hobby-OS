bits 32
section .text
        align 4
        dd 0x1BADB002            
        dd 0x00                  
        dd - (0x1BADB002 + 0x00) 

global start
global keyboard_handler
global syscall_handler
global nic_handler
global read_port
global write_port
global write_port_32
global read_port_32
extern kmain	 
extern keyboard_isr
extern ptr       
extern create_kpgtbl
extern makeGDT
extern syshandler
extern rtl8139_handler

start:
  cli 			;block interrupts
  mov esp, stack_space + 8192
  call makeGDT
  ; Far jump to reload CS with selector 0x08 (code segment)
  jmp 0x08:flush_cs

keyboard_handler:
  call keyboard_isr
  iretd

nic_handler:
  call rtl8139_handler
  iretd

syscall_handler:
  pusha
  push eax
  call syshandler
  add esp, 4
  popa
  iretd

flush_cs:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  call kmain
hang:
  hlt
  jmp hang

read_port:
	mov edx, [esp + 4]
	in al, dx	
	ret

read_port_32:
	mov edx, [esp + 4]
	in eax, dx	
	ret

write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

write_port_32:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, eax  
	ret

section .bss
resb 16384		;16KB for stack
stack_space: