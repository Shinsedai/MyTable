#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

// Creates a new node with the provided data
Node* create_node(Row* data) {
    Node* node = (Node*)malloc(sizeof(Node)); // Allocate memory for the node
    node->data = data;                        // Assign data to the node
    node->left = NULL;                        // Initialize the left child as NULL
    node->right = NULL;                       // Initialize the right child as NULL
    return node;                              // Return the newly created node
}

// Inserts a new node into the binary search tree
Node* insert(Node* root, Row* data) {
    if (root == NULL) {                       // If the tree is empty
        return create_node(data);             // Create and return a new node
    }
    if (data->id < root->data->id) {          // If the data ID is smaller
        root->left = insert(root->left, data); // Recur to the left subtree
    } else if (data->id > root->data->id) {   // If the data ID is larger
        root->right = insert(root->right, data); // Recur to the right subtree
    }
    return root;                              // Return the (unchanged) root node
}

// Searches for a node in the binary search tree by its ID
Row* search(Node* root, int id) {
    if (root == NULL || root->data->id == id) { // If tree is empty or ID matches
        return (root != NULL) ? root->data : NULL; // Return the data or NULL
    }
    if (id < root->data->id) {                // If the ID is smaller
        return search(root->left, id);        // Search in the left subtree
    } else {                                  // Otherwise
        return search(root->right, id);       // Search in the right subtree
    }
}

// Function to find the minimum value node
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

Node* delete(Node* root, int id) {
    if (root == NULL) return root;

    if (id < root->data->id) {
        root->left = delete(root->left, id);
    } else if (id > root->data->id) {
        root->right = delete(root->right, id);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = minValueNode(root->right);

        root->data = temp->data;

        root->right = delete(root->right, temp->data->id);
    }
    return root;
}



// Frees all memory allocated for the binary search tree
void free_tree(Node* root) {
    if (root == NULL) return;                 // If the tree is empty, do nothing
    free_tree(root->left);                    // Free the left subtree
    free_tree(root->right);                   // Free the right subtree
    free(root);                               // Free the current node
}