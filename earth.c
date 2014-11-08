#include<stdio.h>
#include<stdlib.h>

#define SIZE 5

void bubble(int *ar)
{
	for(int i=0;i<SIZE - 1;i++)
	{
		for(int j=i;j<SIZE - 1;j++)
		{
			if(ar[j]>ar[j+1]){
				int keep = ar[j+1];
				ar[j+1]  = ar[j]; 
				ar[j] = keep;
			}
		}
	
	}

}

void straight(int *ar)
{
	for(int j=SIZE;j>0;j--)
	{

		int keep = 0;
		for(int i=j;i>0;i--)
		{
			if(ar[i-1]>ar[i])
			{
				keep = i-1;
			}
		}
		int keep2 = ar[j];
		ar[j] = ar[keep];
		ar[keep] = keep2;

	}
}

void print(int *ar)
{
	for(int i=0;i<SIZE;i++)
	{
		printf("%d ",ar[i]);

	}
	printf("\n");

}

int main(int arg,char **argv)
{
	int a[] = {8,3,9,4,5};
	print(a);
	bubble(a);
	print(a);
	straight(a);
	print(a);
	return 0;
}