#include <stdio.h>
#include <inttypes.h>

#include <capstone/capstone.h>

#define CODE "\x80\xb5\x41\xf2\xd8\x30\x6f\x46\xc0\xf2\x00\x00\x78\x44\x01\xf0\x38\xea\x00\x20\x80\xbd"
/*
80 b5        PUSH    {R7, LR}
41 f2 d8 30  MOVW    R0, #0X13D8    ; clear lower 16 bits
6f 46        MOV     R7, SP
c0 f2 00 00  MOVT.W  R0, #0
78 44        ADD     R0, PC
01 f0 38 ea  BLX     _puts
00 20        MOVS    R0, #0
80 bd        POP     {R7, PC}
*/
int main(void)
{
	csh handle;
	cs_insn *insn;
	size_t count;

	cs_open(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_ARM , &handle);
	count = cs_disasm_ex(handle, (unsigned char *)CODE, sizeof(CODE) - 1, 0x1000, 0, &insn);
	if (count) {
		size_t j;

		for (j = 0; j < count; j++) {
			printf("0x%"PRIx64":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic, insn[j].op_str);
		}

		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);

    return 0;
}


