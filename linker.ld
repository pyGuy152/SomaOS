/* Load at 1 MiB; everything below is BIOS, VGA and memory-mapped devices.
 * .multiboot must be in the first 8 KiB of the file. */

ENTRY(_start)

SECTIONS
{
	. = 1M;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text .text.*)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata .rodata.*)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data .data.*)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss .bss.*)
	}

	/* First free address after the kernel image, for the physical allocator. */
	kernel_end = .;
}
