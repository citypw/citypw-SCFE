	.file	"printf.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"a=%d; b=%d; c=%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	eax, OFFSET FLAT:.LC0
	mov	ecx, 3
	mov	edx, 2
	mov	esi, 1
	mov	rdi, rax
	mov	eax, 0
	call	printf
	mov	eax, 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
