#ifndef _CACHE_H
#define _CACHE_H

typedef unsigned long long int addr_t;
typedef unsigned long int ctag_t;
typedef unsigned int cindex_t;

typedef struct _trace {
    char instruction;
    addr_t address;
    struct _trace *next;
} Trace;

typedef enum _policy {
    UNKNOWN = -1,
    FIFO = 0,
    LRU = 1
} Policy;

typedef struct _block {
    ctag_t tag;
    struct _block *next;
} Block;

void simulateCache(Policy, int, Trace *, int [4]);
void initCache(int, int, int);
void writeToCache(addr_t);
int prefetchAddress(addr_t, int);
void clearCache();
void freeCache();

int checkHit(addr_t, Policy);

cindex_t getSetIndex(addr_t);
ctag_t getTag(addr_t);

#endif
