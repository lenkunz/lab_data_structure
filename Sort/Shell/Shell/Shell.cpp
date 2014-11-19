#include<stdio.h>
#include<conio.h>


typedef int T;
void ShellSort(T * arrs, int arraySize, int shellSizeStart) {
	int shellSize = shellSizeStart;
	while (shellSize > 0){
		for (int shellIndex = 0; shellIndex < shellSize; shellIndex++) {
			for (int i = shellIndex; i + shellSize < arraySize;) {
				if (arrs[i] > arrs[i + shellSize]) {
					int temp = arrs[i];
					arrs[i] = arrs[i + shellSize];
					arrs[i + shellSize] = temp;
					i -= shellSize;
				}
				else
				{
					i = i + shellSize;
				}
			}
		}
		shellSize = shellSize / 2;
	}
}

void print(T * arrs, int size){
	for (int i = 0; i < size; i++){
		printf("%d ", arrs[i]);
	}
	printf("\n");
}

int main(int argc, char * argv){
	T arrs[] = { 0, 0, 7, 21, 15, 9, 8, 14, 13, 19, 19, 99 };
	printf("Start code!\n");
	print(arrs, 12);
	ShellSort(arrs, 12, 13);
	print(arrs, 12);
	getch();
	return 1;
}