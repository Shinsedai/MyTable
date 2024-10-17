#ifndef BTREE_H
#define BTREE_H

typedef struct {
    int id;
    char name[32];
    char email[256];
} Row;

typedef struct Node {
    Row* data;
    struct Node* left;
    struct Node* right;
} Node;

Node* insert(Node* root, Row* data);
Row* search(Node* root, int id);
void free_tree(Node* root);

#endif
