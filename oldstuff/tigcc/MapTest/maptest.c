// C Source File
// Created 1/15/2005; 5:10:22 PM

#include <tigcclib.h>
#include "extgraph.h"
#include "maptest.h"

// Main Function
void _main(void)
{
	clrscr();
	map1.array = map1_array;
	int i, j;
	for(i = 0; i < map1.height; i++) {
		for(j = 0; j < map1.width; j++) {
			printf("%d", map1.array[i][j]);
			int k;
			for(k = 0; k < 1000; k++);
		}
	}
	ngetchx();
	clrscr();
}
