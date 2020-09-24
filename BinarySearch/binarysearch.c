#include <stdio.h>
#include <stdlib.h>
// NOTE: Create function prototypes first before implementing
int isSorted(int arr[], int size);
int binarysearch(int arr[], int size, int value);

int main (int argc, char** argv) {
	const int ARR_SIZE = 10;
	int arr[10] = {1, 4, 7, 8, 9, 16, 17, 18, 19, 210};
	
	if (binarysearch(arr, ARR_SIZE, 8))
		printf("Hello, world!\n");	

	return 0;
}

int isSorted(int arr[], int size) {
	int i = 0;
	
	while (i < size - 1) {
		if (arr[i] <= arr[i + 1])
			++i;
		else
			return 0;
	}
	
	return 1;
}

int binarysearch(int arr[], int size, int value) {
	int left = 0, right = size - 1;

	if (isSorted(arr, size)) {
		while (left < right) {
			int middle  = (right + left / 2);
		
			if (arr[middle] < value) // Too low
				left = middle + 1;
			else if (arr[middle] > value) // Too high
				right = middle - 1;
			else
				return 1;
		}	
	}
	
	return 0;
}
