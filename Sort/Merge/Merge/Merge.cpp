#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int T;

void merge(T * arrs, T * tempArrs, int left, int center, int right);

void MergeSort(T * arrs, T * tempArrs, int left, int right){
	int center = (left + right) / 2;
	if (left < right){
		MergeSort(arrs, tempArrs, left, center);
		MergeSort(arrs, tempArrs, center + 1, right);
		merge(arrs, tempArrs, left, center, right);
	}
}

void merge(T * arrs, T * tempArrs, int left, int center, int right){
	int indexLeft = left, indexRight = center + 1, indexTemp = left;
	while (indexLeft <= center && indexRight <= right){
		if (arrs[indexLeft] < arrs[indexRight]){
			tempArrs[indexTemp++] = arrs[indexLeft++];
		}
		else{
			tempArrs[indexTemp++] = arrs[indexRight++];
		}
	}
	while (indexLeft <= center){
		tempArrs[indexTemp++] = arrs[indexLeft++];
	}
	while (indexRight <= right){
		tempArrs[indexTemp++] = arrs[indexRight++];
	}

	for (indexTemp = left; indexTemp <= right; indexTemp++){
		arrs[indexTemp] = tempArrs[indexTemp];
	}
}

void print(T * arrs, int size){
	for (int i = 0; i < size; i++)
		printf("%d ", arrs[i]);
	printf("\n");
}

int main(int argc, char ** argv){
	T * vb = (T*)malloc(sizeof(T)* 10);
	T * vbT = (T*)malloc(sizeof(T)* 10);

	for (int i = 9, iO = 0; i >= 0; i--, iO++){
		vb[iO] = i;
	}
	
	print(vb, 10);
	MergeSort(vb, vbT, 0, 9);
	print(vb, 10);

	getch();

	return 1;
}