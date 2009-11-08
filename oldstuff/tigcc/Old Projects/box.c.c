// C Source File
// Created 11/1/2004; 6:07:54 PM
#include <stdio.h>

void drawLine(int, char);
void drawWalls(int, char);

// Main Function
int main(void)
{
	int size = 5;
	char symbol = '*';
	printf("give size and char\n");
	scanf("%d%*c%c",&size,&symbol);
	printf("\n");
	drawLine(size, symbol);
	drawWalls(size, symbol);
	drawLine(size, symbol);
	return 0;
}

void drawLine(int l, char c) {
	int i;
	for (i = 1; i <= l; i++) printf("%c", c);
	printf("\n");
}

void drawWalls(int h, char c) {
	int i, ii;
	for (i = 2; i < h; i++) {
		printf("%c", c);
		for (ii = 2; ii < h; ii++) printf(" ");
		printf("%c\n", c);
	}
}
