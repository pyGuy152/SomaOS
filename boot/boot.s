; Multiboot entry point. GRUB/QEMU enters here in 32-bit protected mode with
; paging off, EAX = 0x2BADB002 and EBX = multiboot info pointer.

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
bits 32
global _start
extern kernel_main

_start:
    mov esp, stack_top
    push dword 0
    popfd

    ; EAX/EBX still hold the multiboot magic and info pointer. Push them here
    ; when kernel_main needs the memory map.
    call kernel_main

    cli
.hang:
    hlt
    jmp .hang
