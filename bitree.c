#include <stdlib.h>
#include <string.h>

#include "bitree.h"

void bitree_init(BiTree *tree, void (*destroy)(void *data))
{
    tree->size = 0;
    tree->destroy = destroy;
    tree->root = NULL;
}

void bitree_destroy(BiTree *tree)
{
    bitree_rem_left(tree, NULL);

    memset(tree, 0, sizeof(BiTree));
    return;
}

int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *new_node, **position;

    // 确认哪里要插入节点
    if (node == NULL) {
        // 只有在空树上才允许添加根节点
        if (bitree_size(tree) > 0) {
            return -1;
        }

        position = &tree->root;
    }else {
        // 通常情况下，只允许在分支结束的地方插入
        if (bitree_left(node) != NULL) {
            return -1;
        }

        position = &tree->left;
    }

    // 给节点申请内存
    if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL) {
        return -1;
    }

    // 在树上插入节点
    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;

    tree->size++;

    return 0;
}

int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *new_node, **position;

    // 确认哪里要插入节点
    if (node == NULL) {
        // 只有在空树上才允许添加根节点
        if (bitree_size(tree) > 0) {
            return -1;
        }

        position = &tree->root;
    }else {
        // 通常情况下，只允许在分支结束的地方插入
        if (bitree_right(node) != NULL) {
            return -1;
        }

        position = &tree->right;
    }

    // 给节点申请内存
    if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL) {
        return -1;
    }

    // 在树上插入节点
    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;

    tree->size++;
    return 0;
}

void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    // 空树不允许移除
    if (bitree_size(tree) == 0) {
        return;
    }

    // 确认是哪里要移除
    if (node == NULL) {
        position = &tree->root;
    }else {
        position = &tree->left;
    }

    // 移除节点
    if (*position != NULL) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);

        // 调用用户自定的函数来释放动态分配的数据
        if (tree->destroy != NULL) {
            tree->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;

        tree->size--;
    }

    return;
}

void bitree_rem_right(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;
    // 空树不允许移除
    if (bitree_size(tree) == 0) {
        return;
    }

    // 确认是哪里要移除
    if (node == NULL) {
        position = &tree->root;
    }else {
        position = &tree->right;
    }

    // 移除节点
    if (*position != NULL) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);

        // 调用用户自定的函数来释放动态分配的数据
        if (tree->destroy != NULL) {
            tree->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;

        tree->size--;
    }

    return;
}

int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
    bitree_init(merge, left->destroy);

    // 从已合并的根节点插入数据
    if (bitree_ins_left(merge, NULL, data) != 0) {
        bitree_destroy(merge);
        return -1;
    }

    // 合并两个二进制树到单个的二进制树中
    bitree_root(merge)->left = bitree_root(left);
    bitree_root(merge)->right = bitree_root(right);

    merge->size = merge->size + bitree_size(left) + bitree_size(right);

    // 不要让原树可以访问已合并的树
    left->root = NULL;
    left->size = 0;
    right->root = NULL;
    right->size = 0;

    return 0;
}
