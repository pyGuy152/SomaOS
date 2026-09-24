# SomaOS
#   make        build kernel.bin
#   make run    boot in QEMU
#   make check  verify the multiboot header
#   make clean

CC     := i686-elf-gcc
AS     := nasm
KERNEL := kernel.bin

SRCDIRS  := boot cpu drivers lib kernel
INCLUDES := $(addprefix -I,$(SRCDIRS))

CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -MMD -MP
ASFLAGS := -f elf32
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib

# boot/boot.s stays first so .multiboot lands at the front of the image.
ASM_SRCS := boot/boot.s cpu/gdt_flush.s cpu/idt_load.s
C_SRCS   := kernel/kernel.c cpu/gdt.c cpu/idt.c drivers/vga.c lib/string.c drivers/keyboard.c shell/shell.c drivers/pit.c lib/kprintf.c

OBJS := $(addprefix build/,$(ASM_SRCS:.s=.o) $(C_SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

.PHONY: all run check clean

all: $(KERNEL)

$(KERNEL): $(OBJS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -lgcc
	@echo "Built $@"

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

check: $(KERNEL)
	@grub-file --is-x86-multiboot $(KERNEL) \
		&& echo "$(KERNEL): valid multiboot header" \
		|| echo "$(KERNEL): NOT multiboot (or grub-file not installed)"

clean:
	rm -rf build $(KERNEL)

-include $(DEPS)
