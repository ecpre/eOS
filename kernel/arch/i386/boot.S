# Declarations for multiboot header
.set ALIGN,	1 << 0
.set MEMINFO, 	1 << 1
.set FLAGS,	ALIGN | MEMINFO	
.set MAGIC,	0x1BADB002	#multiboot header magic number
.set CHECKSUM,	-(MAGIC + FLAGS)

#multiboot header
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# allocate stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384
stack_top:

# preallocate pages

.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096

# kernel entry point
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	#physical boot_page_table1 address
	movl $(boot_page_table1 - 0xC0000000), %edi
	#map addr 0
	movl $0, %esi
	#map 1023 pages, 1024 is vga text
	//movl $1023, %ecx
	
1:	
	//cmpl $_kernel_start, %esi
	//jl 2f
	cmpl $(_kernel_end-0xC0000000), %esi
	jge 3f
	#map addresseses as writable
	#todo: map .text and .rodata as non-writable
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)
2:	
	#4k pages
	addl $4096, %esi
	#size of boot_page_table1 entries are 4 bytes
	addl $4, %edi
	loop 1b
3:
	#map vga memory to 0xc03ff00 as writable
	movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4

	# map page table to both 0x0 and 0xc0000000
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	#set cr3 to boot_page_directory addr
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3
	
	#enable paging and write protection
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0
	
	# jump to higher half w/ absolute jump
	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	#paging is now enabled
	#unmap identity mapping
	movl $0, boot_page_directory + 0

	#reload cr4
	movl %cr3, %ecx
	movl %ecx, %cr3

	#setup stack
	mov $stack_top, %esp
	#enter kernel
	pushl %eax
	pushl %ebx
	call _init
	
	popl %ebx
	add $0xC0000000, %ebx
	popl %eax
	pushl %eax
	pushl %ebx	
	
	call kernel_main

	#infinite loop
	
	cli
1:	hlt
	jmp 1b
