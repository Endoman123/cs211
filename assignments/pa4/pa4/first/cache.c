#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "cache.h"

Block **cache;
unsigned int nSets, nBlocks, nBlockBits, nSetBits, sBlock;

void printSet(Block *);
void printCache();

/**
 * Simulates a cache with the given cache properties and trace file,
 * and places the stats in the provided stats array.
 *
 * The stats of the cache in order:
 * - Memory reads
 * - Memory writes
 * - Cache hits
 * - Cache misses
 */
void simulateCache(Policy policy, int prefetchSize, Trace *traces, int stats[4]) {
    int nReads = 0, nWrites = 0, nHits = 0, nMisses = 0;
    Trace *ptr;
    
    for (ptr = traces; ptr != NULL; ptr = ptr->next) {    
        // Attempt cache hit
        if (checkHit(ptr->address, policy))
            ++nHits;
        else { // Miss, we need to write to cache
            ++nMisses;
            ++nReads;

            writeToCache(ptr->address);
           
            nReads += prefetchAddress(ptr->address, prefetchSize);
        }

        // Write-through cache
        // We write to lower memory regardless of cache hit or miss
        if (ptr->instruction == 'W')
             ++nWrites;
    }
     
    // Write cache stats to int array
    stats[0] = nReads;
    stats[1] = nWrites;
    stats[2] = nHits;
    stats[3] = nMisses;
}

/**
 * Check if the address given hits the cache
 *
 * If policy is LRU, will also reposition blocks to keep use order.
 */
int checkHit(addr_t address, Policy policy) {
    cindex_t set = getSetIndex(address);
    ctag_t tag = getTag(address);
    Block *ptr = cache[set], *tail = NULL, *hit = NULL;

    // Find the hit, if there is a hit.
    // If the policy is LRU, we should also remove the ptr from in between.
    while (ptr != NULL) {
        if (hit == NULL && ptr->tag == tag) {
            hit = ptr;
            
            if (policy == LRU) {
                if (tail == NULL) // If tail == NULL, we are at the front of the list
                    cache[set] = hit->next; 
                else // If tail != NULL, we are somewhere in the middle 
                    tail->next = hit->next;
               
                // Since hit is moving to the back,
                // we need to set hit->next to NULL 
                // to keep it from circling to its previous positions 
                hit->next = NULL; 
            }
        } else { // Keep moving if we did not set hit
            tail = ptr;
            ptr = ptr->next;
        }
    }
    
    // Ending adjustments to an LRU hit
    if (policy == LRU) {
        if (cache[set] == NULL) // If there was only one item in the list and we just removed it
            cache[set] = hit;
        else // If there is more than one item in the list
           tail->next = hit; 
    }

    return hit != NULL; 
}

/**
 * Writes the given address to cache
 */
void writeToCache(addr_t address) {
    int i = 0;
    cindex_t set = getSetIndex(address); 
    ctag_t tag = getTag(address);
    Block *ptr;

    if (cache[set] == NULL) {
        cache[set] = malloc(sizeof(Block));
        ptr = cache[set];
    } else {
        for (ptr = cache[set]; ptr != NULL; ptr = ptr->next) { 
            ++i;
            if (ptr->next == NULL) {
                ptr->next = malloc(sizeof(Block));
                ptr = ptr->next;
                break;
            }
        }
    }
  
    // Set the tag for ptr  
    ptr->tag = tag;
    
    // First element is the least relevant element
    // FIFO: this was the firstmost element
    // LRU: this was the least recently used
    if (i == nBlocks) {
        cache[set] = cache[set]->next;
    }
}

/**
 * Prefetch for the given address
 *
 * Returns the number of reads required to prefetch all addresses
 */
int prefetchAddress(addr_t address, int prefetchSize) {
    int i, ret = 0;
    addr_t pfAddr = address;
    for (i = 0; i < prefetchSize; ++i) {
        pfAddr += sBlock;
        
        if (!checkHit(pfAddr, UNKNOWN)) {
            ++ret;
            writeToCache(pfAddr);
        }
    }

    return ret;
}

/**
 * Initialize the cache given the cache properties
 */
void initCache(int cacheSize, int blockSize, int numBlocks) { 
    // Do not try to reinitialize if the cache has not been freed
    if (cache != NULL)
        return;

    // Figure out the number of sets and blocks
    if (numBlocks == -1) { // Fully associative, only one row
        nSets = 1;
        nBlocks = cacheSize / blockSize; 
    } else {
        nBlocks = numBlocks;
        nSets = cacheSize / nBlocks / blockSize;    
    }

    // Initialize cache
    cache = calloc(nSets, sizeof(Block *));
    
    // In case we need it, have the block size kept
    sBlock = blockSize;
    
    // Calculate number of set bits and block bits
    nBlockBits = log2(sBlock);
    nSetBits = log2(nSets);
}

/**
 * Clears the cache by removing all blocks
 */
void clearCache() {
    int i;
    for (i = 0; i < nSets; ++i) {
        while (cache[i] != NULL) {
            Block *temp = cache[i]->next;
            free(cache[i]);    
            cache[i] = temp;
        }
    }
}

/**
 * Completely frees the cache from memory
 */
void freeCache() {
    clearCache(); 
    free(cache);
}

cindex_t getSetIndex(addr_t address) {
    cindex_t mask = (1UL << nSetBits) - 1UL;
    return (address >> nBlockBits) & mask;
}

ctag_t getTag(addr_t address) {
    return (address >> nBlockBits) >> nSetBits;
}

void printCache() {
    int i = 0;
    for (; i < nSets; ++i) {
        printf("%d\t", i);
        printSet(cache[i]);
    }
}

void printSet(Block *set) {
    Block *ptr = set;
    
    for (; ptr != NULL; ptr = ptr->next)
        printf(ptr->next ? "%lx->" : "%lx\n", ptr->tag);
}
