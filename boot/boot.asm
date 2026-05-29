; Magochi v20 GUI Bootloader
[bits 32]

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x03
    dd -(0x1BADB002 + 0x03)

section .bss
align 16
stack_bottom:
    resb 32768
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    push eax
    push ebx
    call kernel_main
    
    cli
.hang:
    hlt
    jmp .hang
