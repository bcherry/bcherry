/*
 * VectorAdd.c
 * 
 * A program to add two arrays of variable length
 *
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */

#include "stdio.h"

void readVector(int vec[], int size);

int main(void) {
    int size;
    scanf("%d",&size);
    while(size) {
        int vec1[size];
        int vec2[size];
        readVector(vec1, size);
        readVector(vec2, size);
        int i;
        for (i = 0; i < size; i++)
            printf("%d ", vec1[i] + vec2[i]);
        printf("\n");
        scanf("%d",&size);
    }
    return 0;
}

void readVector(int vec[], int size) {
    int i;
    for (i = 0; i < size; i++)
        scanf("%d",&vec[i]);
}
