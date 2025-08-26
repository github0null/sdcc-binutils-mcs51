#include <mcs51/at89x52.h>
#include "main.h"

static void same_name_func(void)
{
	__asm__("nop");
	__asm__("nop");
	__asm__("nop");
}

/* not dead code */
int foo(int a)
{
	same_name_func();

	for (int i = 0; i < a; i++) {	
		P0_2 != P0_2;
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
	}

	return P0_2 ? (a * 123 + 456) : (a * 789);
}

/* dead code will be deleted */
int bar(int a)
{
	for (int i = 0; i < a; i++) {	
		P1_1 != P1_1;
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
	}

	return P1_1 ? (a * 789 + 234) : (a * 123);
}
