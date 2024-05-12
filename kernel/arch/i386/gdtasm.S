.global x86_32_GDT_load
x86_32_GDT_load:
	mov 4(%esp), %edx
	mov 8(%esp), %eax
	lgdt (%edx)
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %gs
	mov %eax, %fs
	mov %eax, %ss

	pushl 12(%esp)
	push $.reload_cs
	ljmp *(%esp)

.reload_cs:
	add $8, %esp
	ret

