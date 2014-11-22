#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int T;
int quick(T * arrs, int start, int end);

void QuickSort(T * arrs, int start, int end){
	if (start < end){
		int center = quick(arrs, start, end);
		QuickSort(arrs, start, center - 1);
		QuickSort(arrs, center + 1, end);
	}
}

int quick(T * arrs, int start, int end){
	int valueUse = arrs[start], indexFront = start + 1, indexEnd = end;
	while (indexFront < indexEnd) {
		if (arrs[indexFront] < valueUse) {
			indexFront++;
		}
		else if (arrs[indexEnd] < valueUse) {
			int temp = arrs[indexEnd];
			arrs[indexEnd] = arrs[indexFront];
			arrs[indexFront] = temp;
			indexFront++;
			indexEnd--;
		}
		else {
			indexEnd--;
		}
	}

	while (arrs[indexEnd] > valueUse){
		indexEnd--;
	}

	arrs[start] = arrs[indexEnd];
	arrs[indexEnd] = valueUse;
	return indexEnd;
}

void print(T * arrs, int size){
	for (int i = 0; i < size; i++){
		printf("%d ", arrs[i]);
	}
	printf("\n");
}

int main(int argc, char * argv){
	T arrs[] = { 50, 38, 31, 2, 91, 35, 2, 40, 20, 48 };
	printf("Start code!\n");
	print(arrs, 10);
	QuickSort(arrs, 0, 9);
	print(arrs, 10);
	getch();
}
