/*
 * Lab 16: vectorAdd
 * Author: Torin Kampa (tkampa@ucsc.edu)
 */

#include<stdio.h>

// prototypes
void readVector(int vec[], int size);
void printSumVector(int vecOne[], int vecTwo[], int size);

int main(){
	int size = -1;
	while(size != 0){
		scanf("%d", &size);
		if(size != 0){
			int vecOne[size];
			int vecTwo[size];
			readVector(vecOne, size);
			readVector(vecTwo, size);
			printSumVector(vecOne, vecTwo, size);
		}
	}
    return 0;
}

// read in an array of integers of legth size
void readVector(int vec[], int size){
	int i;
    for(i=0; i<size; i++)
		scanf("%d", &vec[i]);
}

// print out the sum of two integer arrays of length size
void printSumVector(int vecOne[], int vecTwo[], int size){
	int i;
    for(i=0; i<size; i++)
		printf("%d ", vecOne[i] + vecTwo[i]);
	printf("\n");
}
