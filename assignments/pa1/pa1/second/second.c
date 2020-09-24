#include <stdio.h>
#include <stdlib.h>

// Linked List
typedef struct _Node {
    int value;
    struct _Node* next;
} Node;

int printList();
Node* initNode(int v);
int insert(int v);
int delete(int v);

Node* list;
int count = 0;

int main(int argc, char** argv) {
    FILE *fp = fopen(argv[1], "r");
	
    if (fp != NULL) {
        char type;
		int i;

		while(fscanf(fp, "%c\t%d\n", &type, &i) > 0) {
            switch(type) {
                case 'i': // insert
                    insert(i);
                    break;
                case 'd': // delete
                    delete(i);
                    break;
                default:
                    break;
            }
        }
		
		fclose(fp);
	}

    printList();
	
	return 0;
}

/**
 * Inserts a new node into the list if there is no duplicate
 */
int insert(int v) {
    Node* new = initNode(v);

    if (count == 0) { // List is empty
        list = new;
    } else if (list->value > v) { // The new element is the smallest element
        Node* temp = list;
        list = new;
        new->next = temp;
    } else {
        for (Node* current = list; current != 0; current = current->next) {
            if (current->value == v) // If the values match, don't bother continuing the search
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

    ++count;

    return 0;
}

/**
 * Deletes all instances of the value in the list
 *
 * Assumes that the list is sorted
 */
int delete(int v) {
    if (count > 0) { // Only bother if the list has things in it
        if (list->value == v) {
             // Remove and free
             Node* toRemove = list;
             list = list->next;

             if (toRemove != 0)
                 free(toRemove);
                
            --count;
        } else {
            for (Node* current = list; current != 0; current = current->next) { // Find the beginnging instance of the value
                if (current->next != 0 && current->next->value == v) {
                    // Remove and free
                    Node* toRemove = current->next;
                    current->next = current->next->next;
                        
                    if (toRemove != 0)
                        free(toRemove);

                    --count;
                
                    break;
                }
            }
        }
    }
    
    return 0;
}

int printList() {
    printf("%d\n", count);

    for (Node* current = list; current != 0; current = current->next) 
        printf("%d\t", current->value);

    printf("\n");
    
    return 0;
}

/**
 * Creates a new node in memory
 */
Node* initNode(int v) {
    Node* node = malloc(sizeof(Node));

    node->value = v;

    return node;
}
