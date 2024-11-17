#ifndef BTREE_H
#define BTREE_H

// Definition of the Row structure to store data
typedef struct {
    int id;               // Unique identifier for the row
    char name[32];        // Name field
    char email[256];      // Email field
} Row;

// Definition of the Node structure for the binary tree
typedef struct Node {
    Row* data;            // Pointer to the Row data
    struct Node* left;    // Pointer to the left child
    struct Node* right;   // Pointer to the right child
} Node;

// Function declarations for binary tree operations
Node* insert(Node* root, Row* data); // Inserts data into the tree
Row* search(Node* root, int id);     // Searches for a node by ID
void free_tree(Node* root);          // Frees all memory used by the tree

#endif
