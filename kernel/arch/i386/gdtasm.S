.global x86_32_GDT_load
x86_32_GDT_load:

	
	//movl %esp, %ebp
	
	ret
/*	
	//load gdt
	movl +8(%ebp), %eax
		
	lgdt (%eax)

	movl +12(%ebp), %eax
	pushl %eax
	
	call .reload_cs
	retf

/*
.reload_cs:

	mov +16(%ebp), %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	movl %ebp, %esp
	popl %ebp
	ret
	
*/
