ENTRY(_start)

SECTIONS {
	. = 0x00100000;
	
	_kernel_start = .;

	.multiboot.data : {
		*(.multiboot.data)
	}

	.multiboot.text : {
		*(.multiboot.text)
	}

	. += 0xC0000000;

	.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000) {
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000) {
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000) {
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000) {
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	
	/* this is probably a bad implementation of sorts */

	_mmap_start = ALIGN(4096);
		
	_mmap_end = _mmap_start + 0x00100000;

	. = _mmap_end;

	. = . + 0x1000;


	_kernel_end = .;

}
