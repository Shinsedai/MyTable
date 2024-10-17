#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

Node* create_node(Row* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insert(Node* root, Row* data) {
    if (root == NULL) {
        return create_node(data);
    }
    if (data->id < root->data->id) {
        root->left = insert(root->left, data);
    } else if (data->id > root->data->id) {
        root->right = insert(root->right, data);
    }
    return root;
}

Row* search(Node* root, int id) {
    if (root == NULL || root->data->id == id) {
        return (root != NULL) ? root->data : NULL;
    }
    if (id < root->data->id) {
        return search(root->left, id);
    } else {
        return search(root->right, id);
    }
}

void free_tree(Node* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}