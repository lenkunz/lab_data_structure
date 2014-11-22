#include <stdio.h>
#include <conio.h>

void swap(int * heap, int a, int b){
	int temp = heap[a];
	heap[a] = heap[b];
	heap[b] = temp;
}

void siftup(int * heap, int start, int end){
	int child = end;
	while (child > start){
		int parent = (child - 1) / 2;
		if (heap[parent] < heap[child]){
			swap(heap, parent, child);
			child = parent;
		} else return;
	}
}

void siftdown(int * heap, int start, int end){
	int root = start;
	while (root * 2 + 1 <= end){
		int child = root * 2 + 1;
		int swapi = root;

		if (heap[swapi] < heap[child]){
			swapi = child;
		}
		if (child + 1 <= end && heap[swapi] < heap[child + 1]){
			swapi = child + 1;
		}
		if (swapi == root){
			return;
		} else {
			swap(heap, root, swapi);
			root = swapi;
		}
	}
}

void heapify(int * heap, int size){
	int end = 1;

	while (end < size){
		siftup(heap, 0, end);
		++end;
	}
}

void HeapSort(int * heap, int size){
	heapify(heap, size);
	int end = size - 1;

	while (end > 0){
		swap(heap, 0, end);
		--end;

		siftdown(heap, 0, end);
	}
}

void print(int * arrs, int size){
	for (int i = 0; i < size; i++)
		printf("%d ", arrs[i]);
	printf("\n");
}

void main(){
	int heap[] = {6,31,35,2,3,15,56,18,23};
	print(heap, 9);
	HeapSort(heap, 9);
	print(heap, 9);
	getch();
}