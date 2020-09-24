#include <stdio.h>
#include <stdlib.h>

// Right Truncatable Prime
int isRightTrunctablePrime(int a);
int main (int argc, char** argv) {
	FILE *fp = fopen(argv[1], "r");
	if (fp != NULL) {
		int i, a;
		fscanf(fp, "%d", &i);
		
		for (; i > 0; --i) {
			fscanf(fp, "%d", &a);
			printf(isRightTrunctablePrime(a) ? "yes\n" : "no\n");
		}
	
		fclose(fp);
	}
	
	return 0;
}

/**
 * Checks if number is right trunctable prime
 * Returns 1 if RTP, 0 otherwise 
 */
int isRightTrunctablePrime(int a) {
	while (a > 0) {
		for (int i = 2; i * i <= a; i++) {
			if (a % i == 0)
				return 0;
		}
		
		a /= 10;
	}

	return 1;
}
