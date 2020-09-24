#include <stdio.h>
#include <stdlib.h>

char* convertToPigLatin(char*);
int isVowel(char);

int main (int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        char* pig = convertToPigLatin(argv[i]);
        printf("%s", pig);

        if (i < argc - 1)
            printf(" ");
        else
            printf("\n");
    }
	
	return 0;
}

/**
 * Convert a string to pig latin
 *
 * RULES:
 * - Words starting with vowels simply add "yay" to the end of their word
 * - Otherwise, move all the letters up to the first vowel to the end and add "ay" 
 */
char* convertToPigLatin(char* string) {
    char* ret = malloc(sizeof(string) + 16 * sizeof(char));

    if (isVowel(string[0])) {
        int i = 0;
        for (; string[i] != '\0'; ++i)
            ret[i] = string[i];

        ret[i] = 'y';
        ret[i + 1] = 'a';
        ret[i + 2] = 'y';
        ret[i + 3] = '\0';
    } else {
        int i = 0, j = 0, foundVowel = 0;
        
        for (; string[j] != '\0'; ++j) {
            if (!foundVowel && isVowel(string[j]))
                foundVowel = 1;
            
            if (foundVowel) {
                ret[i] = string[j];
                ++i; 
            }
        }

        for (j = 0; !isVowel(string[j]); ++j) {
            ret[i] = string[j];
            ++i;
        }

        ret[i] = 'a';
        ret[i + 1] = 'y';
        ret[i + 2] = '\0';
    } 

	return ret;
}

/**
 * Gets whether or not a letter is a vowel
 */
int isVowel(char c) {
    char vowels[] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};

    for (int i = 0; i < 10; ++i) {
        if (c == vowels[i])
            return 1;
    }
    
    return 0;
} 
