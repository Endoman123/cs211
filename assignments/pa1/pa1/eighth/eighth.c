#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int value;
    struct _Node *left, *right;
} Node;

Node* insert(Node *, int, int*);
Node* createNode(int);
int search(Node *, int);
int freeTree(Node*);

int main (int argc, char** argv) {
    FILE *fp = fopen(argv[1], "r");

    if (fp != 0) { 
        Node *tree = NULL;
        int value = 0;
        char mode = ' ';
       
        while (fscanf(fp, "%c\t%d\n", &mode, &value) > 0) {
            int height = 1;
            switch(mode) {
                case 'i':;
                    Node* newTree = insert(tree, value, &height);

                    if (newTree !=  0) {
                        tree = newTree;
                        printf("inserted %d\n", height);
                    } else
                        printf("duplicate\n");

                    break;
                case 's':;
                    height = search(tree, value);
                    
                    if (height > 0)
                        printf("present %d\n", height);
                    else
                        printf("absent\n"); 
            }
        }

        freeTree(tree);
    } else 
       printf("Error\n"); 

    return 0;
}

/**
 * Inserts a value into the BST if it does not exist in the tree.
 * Returns a modified tree with the value inserted at the root if successful,
 * null otherwise
 */
Node* insert(Node* root, int value, int *height) {
    // If the root is null, this means this is the
    // empty space that we can insert it at
    if (root == 0) { 
        return createNode(value);
    } else if (root->value > value) { // If the value smaller than the root's 
        ++*height;
        Node *newNode= insert(root->left, value, height);
        
        if (newNode != 0)
            root->left = newNode;
        else
            return 0;    
    } else if (root->value < value) { // If the value is larger than the root's
        ++*height;
        Node *newNode = insert(root->right, value, height);
        
        if (newNode != 0)
            root->right = newNode;
        else
            return 0; 
    } else // Duplicate value; don't add.
        return 0;

    return root; // By default, return itself
}

/**
 * Allocates a new node into memory and sets its value.
 */
Node* createNode(int value) {
    Node* ret = malloc(sizeof(Node));

    ret->value = value;

    return ret;
}

/**
 * Searches for the specified value in the given tree.
 * Returns the height found if present, 0 if absent.
 */
int search(Node* root, int value) {
    if (root == 0) 
        return 0;
    else if (root->value != value) {
        int height = search(root->value > value ? root->left : root->right, value);
        
        return height > 0 ? 1 + height : 0;
    } else
        return 1;
}

/**
 * Frees the given tree from memory
 */
int freeTree(Node* root) {
    if (root != 0) {
        freeTree(root->left);
        freeTree(root->right);
    
        free(root);
    }    

    return 0;
}
