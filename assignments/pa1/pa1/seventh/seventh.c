#include <stdio.h>
#include <stdlib.h>

char getLast(char*);

int main (int argc, char** argv) {
    for (int i = 1; i < argc; i++)
        printf("%c", getLast(argv[i])); 
	
    printf("\n");

	return 0;
}

/**
 * Gets the last character in a string
 */
char getLast(char* string) {
    int i = 0;
    
    while (string[i + 1] != '\0')
        ++i;

    return string[i];
}
