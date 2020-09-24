#include <stdio.h>
#include <stdlib.h>

int array[10] = {3, 5, 6, 8, 7, 42, 36, 87, 110};

int linearsearch(int key) {
	int i = 0;

	for (i = 0; i < 10; i++) {
		if (key == array[i]) return 1;
	}
	
	return 0;
}

int main (int argc, char** argv) {
	if (linearsearch(5))
		printf("Hello, world!\n");	

	return 0;
}
