#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b);

int main(int argc, char** argv) {
	int p = 7, q = 42;

	printf("p = %d\nq = %d\n", p, q);
	printf("Swapping p and q...\n");
	swap(&p, &q);
	printf("p = %d\nq = %d\n", p, q);
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;

	return;
}
