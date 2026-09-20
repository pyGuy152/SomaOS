; void gdt_flush(struct gdt_ptr *gp);
;
; The CPU caches a descriptor when its segment register is written, so the old
; GDT stays in effect until every selector is rewritten. CS cannot be set with
; MOV, hence the far jump.

section .text
bits 32
global gdt_flush

gdt_flush:
    mov  eax, [esp + 4]
    lgdt [eax]

    mov  ax, 0x10
    mov  ds, ax
    mov  es, ax
    mov  fs, ax
    mov  gs, ax
    mov  ss, ax

    jmp  0x08:.flush
.flush:
    ret
