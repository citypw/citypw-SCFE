.section .data
	true: .asciz "ok.\n"
	true_len = . - true
	false: .asciz "syntax error.\n"
	false_len = . - false

.section .text

.global _start
_start:
	movl $2, %eax
	cmpl $1, %eax
	jg greater
	
	movl $true_len, %edx
	movl $true, %ecx
	movl $1, %ebx
	movl $4, %eax
	int $0x80

	movl $0, %ebx
	movl $1, %eax
	int $0x80

greater:
	movl $false_len, %edx
	movl $false, %ecx
	movl $1, %ebx
	movl $4, %eax
	int $0x80	
