.global x86_32_IDT_load
.extern idt_desc
x86_32_IDT_load:
	lidt (idt_desc)
	ret

