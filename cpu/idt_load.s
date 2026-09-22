section .text
bits 32

; void idt_load(struct idt_ptr *ip);
global idt_load
idt_load:
    mov  eax, [esp + 4]
    lidt [eax]
    ret

; Vector 0, #DE. The CPU has already pushed EFLAGS, CS and EIP; vector 0 pushes
; no error code. What the stub builds on the stack is `struct registers`.
extern fault_handler
global isr0

isr0:
    cli
    pusha

    push ds
    push es
    push fs
    push gs

    ; The interrupted code may have been ring 3 with its own segments.
    mov  ax, 0x10
    mov  ds, ax
    mov  es, ax
    mov  fs, ax
    mov  gs, ax

    mov  eax, esp
    push eax
    call fault_handler
    add  esp, 4

    pop  gs
    pop  fs
    pop  es
    pop  ds
    popa

    ; iret restores EFLAGS, so no sti is needed.
    iret

global irq0
extern pit_handler

irq0:
    cli
    pusha                 
    call pit_handler 
    popa                  
    sti
    iret

global irq1
extern keyboard_handler

irq1:
    cli
    pusha                 
    call keyboard_handler 
    popa                  
    sti
    iret                  

; Adding vectors:
;   1. Vectors 8, 10-14 and 17 have a CPU-pushed error code. Push a dummy 0 in
;      the others so every handler sees one layout.
;   2. Hardware IRQs must send an EOI: outb(0x20, 0x20), plus outb(0xA0, 0x20)
;      for IRQ 8-15, or the IRQ never fires again.
