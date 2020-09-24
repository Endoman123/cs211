#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cache.h"

Policy getPolicy(char *);
int isValidSize(int);
int getNumBlocks(char *);

int main(int argc, char** argv) {
    int cacheSize = -1, blockSize = -1, prefetchSize = -1, numBlocks = 0;
    Policy policy = UNKNOWN;
    char *c_policy = malloc(5 * sizeof(char)), *associativity = malloc(9 * sizeof(char));
    FILE *traceFile;
    
    if (argc != 7) {
        printf("Usage: ./first <cache size><block size><cache policy><associativity><prefetch size><trace file>\n\n");
        return -1;
    }
   
    // Initialize cache info
    sscanf(argv[1], "%d", &cacheSize);
    sscanf(argv[2], "%d", &blockSize);
    sscanf(argv[3], "%s", c_policy);
    sscanf(argv[4], "%s", associativity);
    sscanf(argv[5], "%d", &prefetchSize);

    if (!isValidSize(cacheSize) || // Cache is a positive power of 2 
        !isValidSize(blockSize) || // block size is a positive power of 2
        cacheSize < blockSize || // Cache size is bigger than the cache block size
        prefetchSize < 1 || // Prefetch size must be positive non-zero
        (policy = getPolicy(c_policy)) == UNKNOWN || // Policy must be either FIFO or LRU
        !(numBlocks = getNumBlocks(associativity)) || // The number of blocks must not be 0
        access(argv[6], R_OK) == -1) { // The trace file must not only exist, but must be readable
        printf("error\n");
        return -1;
    } 
   
    // Open the trace file 
    traceFile = fopen(argv[6], "r");

    // Technically we don't need this check,
    // but it doesn't hurt to have it
    if (traceFile != 0) {
        int stats[4];
        Trace *traces = malloc(sizeof(Trace)), *ptr = 0;
        char instr;
        addr_t address;

        while (fscanf(traceFile, "%c %llx ", &instr, &address) == 2) {
            if (instr == '#') // EOF
                break;
            
            if (ptr == 0) 
                ptr = traces;
            else {
                ptr->next = malloc(sizeof(Trace));
                ptr = ptr->next;
            }

            ptr->instruction = instr;
            ptr->address = address;
        }

        // Initialize the cache
        initCache(cacheSize, blockSize, numBlocks);

        // Simulate the cache without prefetching
        printf("no-prefetch\n");
        simulateCache(policy, 0, traces, stats);
        printf("Memory reads: %d\n", stats[0]);
        printf("Memory writes: %d\n", stats[1]);
        printf("Cache hits: %d\n", stats[2]);
        printf("Cache misses: %d\n", stats[3]);
        
        clearCache();

        // Simulate the cache with prefetching
        printf("with-prefetch\n"); 
        simulateCache(policy, prefetchSize, traces, stats);
        printf("Memory reads: %d\n", stats[0]);
        printf("Memory writes: %d\n", stats[1]);
        printf("Cache hits: %d\n", stats[2]);
        printf("Cache misses: %d\n", stats[3]);
        
        // Free cache and traces when finished
        freeCache();
        while (traces != 0) {
            Trace *ptr = traces->next;
            free(traces);
            traces = ptr;
        }
    }

    return 0;
}


/**
 * Gets whether or not a value is a valid storage size for caches.
 * Valid sizes are powers of 2 greater than 1 
 */
int isValidSize(int num) {
    if (num <= 0)
        return 0;

    for (; num > 1; num /= 2) {
        if (num % 2 == 1)
            return 0;
    }

    return 1;
}

/**
 * Gets the policy given by the string
 */
Policy getPolicy(char *str) {
    char lower[5];
    int i;

    for (i = 0; i < 4 && str[i] != 0; ++i)
        lower[i] = tolower(str[i]);

    lower[4] = '\0'; 
    
    if (!strcmp(lower, "fifo"))
        return FIFO;
    else if (!strcmp(lower, "lru"))
        return LRU;

    return UNKNOWN;
}

/**
 * Gets the number of blocks for the associativity mapping,
 * -1 if fully associative.
 *  
 * Returns the number of blocks, 0 otherwise.
 * Valid associativities: direct, assoc, assoc:n 
 */
int getNumBlocks(char *str) {
    char lower[7];
    int i, ret = 0;

    for (i = 0; i < 7 && str[i] != 0 && str[i] != ':'; ++i)
        lower[i] = tolower(str[i]);

    lower[i] = '\0';

    if (!strcmp(lower, "direct")) // Direct mapping is the same as one-way association
        ret = 1;
    else if (!strcmp(lower, "assoc")) {
        ret = -1;

        sscanf(str, "assoc:%d", &ret); 
    }

    return ret;
}
