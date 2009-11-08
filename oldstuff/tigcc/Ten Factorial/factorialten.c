// C Source File
// Created 1/10/2005; 2:43:54 PM

#include <tigcclib.h>

// Main Function
void _main(void)
{
	clrscr();
	long total = 1;
	int x;
	for (x = 1; x <= 10; total*=x++);
	printf("10! is %lu", total);
	ngetchx();
}
