#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int value;
    struct _Node *left, *right;
} Node;

Node* insert(Node *, int, int *);
Node* delete(Node *, int, int *);
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
                    
                    break;
                case 'd':;
                     int success = 0;

                     tree = delete(tree, value, &success);

                     printf("%s\n", success ? "success" : "fail"); 
                break;
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
Node *insert(Node *root, int value, int *height) {
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
 * Deletes a node from the tree if present.
 * Returns a pointer to the modified tree if successful.
 */
Node* delete(Node *root, int value, int *success) {
    if (root == 0) // Can't delete a null node, return itself.
        return root;

    if (root->value > value) // Root's value is larger (left subtree)
        root->left = delete(root->left, value, success);  
    else if (root->value < value) // Root's value is smaller (right subtree)
        root->right =  delete(root->right, value, success);
    else { // Found the node
        Node *newRoot = 0;
        // If the node has one to no siblings, 
        // use the non-null node child or null the root.
        if (root->right == 0) {
            newRoot = root->left;
            free(root);
        } else if (root->left == 0) {
            newRoot = root->right;
            free(root);
        } else { // Node has two siblings, must find a node replacement
            // The replacement is simply the smallest node in the right subtree.
            Node* temp = root->right;
             
            while(temp->left != 0)
                temp = temp->left;

            // Just use the node's value and delete it.
            root->value = temp->value;
            root->right = delete(root->right, root->value, success);
            
            // Return the old root as the new root
            newRoot = root;
        } 

        // Mark as a success and return
        *success = 1;
        return newRoot;
    }
    
    return root;   
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
