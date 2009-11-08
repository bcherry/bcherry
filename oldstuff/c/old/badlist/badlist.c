#include "stdlib.h"
#include "stdio.h"

#define NUM_CHOICES 9

int main() {
	printf("What did JC do this time?\n");
	char* choices[] = {
		"played with a lighter",
		"disobeyed a direct order",
		"slept on the bone table",
		"didn't listen to Ricardo",
		"was worse than Eric",
		"cried",
		"sucked up to Jon",
		"left for Philmont",
		"sucked balls"
	};
	int i;
	for(i = 0; i < NUM_CHOICES; i++)
		printf("%d - %s\n",i+1,choices[i]);
	int in;
	scanf("%d",in);
	printf("JC is on the bad list\n");
	return 0;
}
