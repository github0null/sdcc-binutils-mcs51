void dummy() __naked
{
    __asm
	.global _delay100ms
	.section .text, "ax"
	.using 0
_delay100ms:
	push    ar0
	push    ar1
	mov     r0, #193
	mov     r1, #125
.L00001:
	djnz    r1, .L00001
	djnz    r0, .L00001
	pop     ar1
	pop     ar0
	ret
    __endasm;
}
