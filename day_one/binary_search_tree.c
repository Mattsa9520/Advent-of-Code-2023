#include "binary_search_tree.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BinarySearchTree {
    Node* root;
} BinarySearchTree;

static void _Node_Insert(Node** node, int value) {
    if (node == NULL) return;

    if (*node == NULL) {
        *node = malloc(sizeof(Node));
        (*node)->val = value;
        (*node)->left = NULL;
        (*node)->right = NULL;
        return;
    }

    Node* n = *node;

    if (value > n->val) {
        _Node_Insert(&n->right, value);
    } else {
        _Node_Insert(&n->left, value);
    }
}

static int* _Node_Min(Node* node) {
    if (node == NULL) return NULL;

    if (node->left != NULL) {
        return _Node_Min(node->left);
    } else  {
        return &node->val;
    }
}

static int* _Node_Max(Node* node) {
    if (node == NULL) return NULL;

    if (node->right != NULL) {
        return _Node_Max(node->right);
    } else {
        return &node->val;
    }
}

static void _Node_Destory(Node* node) {
    if (node == NULL) return;

    Node* left = node->left;
    Node* right = node->right;

    free(node);

    _Node_Destory(left);
    _Node_Destory(right);
}

BinarySearchTree* BST_Create() {
    BinarySearchTree* bsp = malloc(sizeof(BinarySearchTree));
    if (bsp == NULL) {
        fprintf(stderr, "MALLOC ERROR: Failed to initialize Binary Search tree!\n");
        exit(0);
    }

    bsp->root = NULL;
    return bsp;
}

void BST_Insert(BinarySearchTree* tree, int value) {
    if (tree == NULL) return;

    _Node_Insert(&tree->root, value);
}

int* BST_Min(const BinarySearchTree* tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    return _Node_Min(tree->root);
}

int* BST_Max(const BinarySearchTree* tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    return _Node_Max(tree->root);
}

void BST_Destory(BinarySearchTree* tree) {
    if (tree == NULL) return;

    _Node_Destory(tree->root);

    free(tree);
}
