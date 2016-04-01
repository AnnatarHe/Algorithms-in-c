/**
 * 二叉树
 */
#ifndef BITREE_H
#define BITREE_H
#include <stdlib.h>

typedef struct BiTreeNode_
{
    void *data;
    struct BiTreeNode_ *left;
    struct BiTreeNode_ *right;
} BiTreeNode;

typedef struct BiTree_
{
    int size;
    int (*compare)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    BiTreeNode *root;
} BiTree;

void bitree_init(BiTree *tree, void (*destroy)(void *data));
void bitree_destroy(BiTree *tree);
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);
