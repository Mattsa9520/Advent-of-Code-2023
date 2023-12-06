#pragma once

typedef struct BinarySearchTree BinarySearchTree;

BinarySearchTree* BST_Create();

void BST_Insert(BinarySearchTree* tree, int value);

int* BST_Min(const BinarySearchTree* tree);

int* BST_Max(const BinarySearchTree* tree);

void BST_Destory(BinarySearchTree* tree);
