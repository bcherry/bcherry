#include "stdio.h"

void barChartH(int[]);
void barChartV(int[]);
int max(int*);

int main(void) {
    int data[] = {10,15,13,23,45,23,34,6,33,4,3,2,34,54,65};
    char hOrV;
    printf("Horizontal or Vertical?\n");
    scanf("%c",&hOrV);
    if (hOrV == 'h') barChartH(data);
    else if (hOrV == 'v') barChartV(data);
    return 0;
}

void barChartH(int data[]) {
    int i, j;
    for (i = max(data); i > 0; i--) {
        for (j = 0; j < 15; j++) {
            if (data[j] >= i)
                printf("*");
            else printf(" ");
        }
        printf("\n");
    }
}

void barChartV(int data[]) {
    int i, j;
    for (i = 0; i < 15; i++) {
        for (j = 0; j < data[i]; j++)
            printf("*");
        printf("\n");
    }
}

int max(int data[]) {
    int i, max = data[0];
    for (i = 1; i < 15; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}
