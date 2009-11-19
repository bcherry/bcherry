#include "ti86asm.inc"

.org _asm_exec_ram

	nop
	jp ProgStart
	.dw 0
	.dw ShellTitle

ShellTitle:
	.db "New Assembly Program",0

ProgStart:



.end
