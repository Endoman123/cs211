#include <stdio.h>
#include <stdlib.h>

// Linked List
typedef struct _Node {
    int value;
    struct _Node* next;
} Node;

Node* initNode(int v);
int insert(int v);
int search(int v);
int hash(int v);

Node* hashTable[1000];

int main(int argc, char** argv) {
    FILE *fp = fopen(argv[1], "r");
  
    if (fp != NULL) {
        char type;
    	int i;
 
  	    while(fscanf(fp, "%c\t%d\n", &type, &i) > 0) {
            int ret;
            switch(type) {
                case 'i': // insert
                    ret = insert(i);
                    printf("%s\n", ret == 1 ? "inserted" : "duplicate");
                    break;
                case 's': // search
                    ret = search(i);
                    printf("%s\n", ret == 1 ? "present" : "absent");
                    break;
                default:
                    break;
            }
        }
	
	    fclose(fp);
    }
	
	return 0;
}

/**
 * Inserts a new node into the list if there is no duplicate
 *
 * Returns 1 if successful, 0 if there is a duplicate
 */
int insert(int v) {
    Node* new = initNode(v);
    int hashCode = hash(v);

    if (hashTable[hashCode] == 0) { // List is empty
        hashTable[hashCode] = new;
    } else if (hashTable[hashCode]->value > v) { // The new element is the smallest element
        Node* temp = hashTable[hashCode];
        hashTable[hashCode] = new;
        new->next = temp;
    } else {
        for (Node* current = hashTable[hashCode]; current != 0; current = current->next) { 
            if (current->value == v) // Don't bother continuing after finding a duplicate
                return 0;

            if (current->next == 0) { // At the end, did not fit anywhere behind it
                current->next = new;
                break;
            } else if (current->next->value > v) { // If it fits, it sits.
                Node* temp = current->next;
                current->next = new; 
                new->next = temp;

                break;
            } 
        }
    } 

    return 1;
}

/**
 * Searches for a value in the hash table
 *
 * Returns 1 if found, 0 if not
 */
int search(int v) {
    for (Node* current = hashTable[hash(v)]; current != 0; current = current->next) { // Find the beginnging instance of the value
        if (current->value == v)
            return 1; 
    }
    

    return 0;
}

/**
 * Gets the hashcode of a value
 */
int hash(int v) {
    int hash = v % 1000;

    if (hash < 0)
        hash *= -1;

    return hash;
}

/**
 * Creates a new node in memory
 */
Node* initNode(int v) {
    Node* node = malloc(sizeof(Node));

    node->value = v;

    return node;
}
