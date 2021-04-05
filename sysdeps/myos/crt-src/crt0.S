.section .text

.global _start
_start:
	movl $0, %ebp
	movl %esp, %eax
	pushl $main
	pushl %eax
	call __mlibc_entry
	ud2
.size _start, . - _start
