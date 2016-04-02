#include <stdlib.h>
#include <string.h>

#include "bistree.h"

static void destroy_right(BisTree *tree, BiTreeNode *node);

// 左旋转
static void rotate_left(BiTreeNode **node)
{
    BiTreeNode *left, *grandchild;

    left = bitree_left(*node);

    // 执行LL转换
    if (((AvlNode *)bitree_data(left))->factor == AVL_LFT_HEAVY) {
        bitree_left(*node) = bitree_right(left);
        bitree_right(left) = *node;
        ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
        ((AvlNode *)bitree_data(*left))->factor = AVL_BALANCED;
        *node = left;
    }else {
        // 执行LR转换
        grandchild = bitree_right(left);
        bitree_right(left) = bitree_right(grandchild);
        bitree_left(grandchild) = left;
        bitree_left(*node) = bitree_right(grandchild);
        bitree_right(grandchild) = *node;
        
        switch (((AvlNode *)bitree_data(grandchild))->factor) {
            case AVL_LFT_HEAVY:
                ((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
                ((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
                break;
            case AVL_BALANCED:
                ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
                break;
            case AVL_RGT_HEAVY:
                ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AvlNode *)bitree_data(left))->factor = AVL_LFT_HEAVY;
                break;
        }

        ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
        *node = grandchild;
    }
    return;
}

static void rotate_right(BiTreeNode **node)
{
    BiTreeNode *right, *grandchild;

    right = bitree_right(*node);

    // 执行LL转换
    if (((AvlNode *)bitree_data(right))->factor == AVL_LFT_HEAVY) {
        bitree_right(*node) = bitree_left(right);
        bitree_left(left) = *node;
        ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
        ((AvlNode *)bitree_data(*right))->factor = AVL_BALANCED;
        *node = right;
    }else {
        // 执行LR转换
        grandchild = bitree_left(right);
        bitree_left(right) = bitree_left(grandchild);
        bitree_right(grandchild) = right;
        bitree_right(*node) = bitree_left(grandchild);
        bitree_left(grandchild) = *node;
        
        switch (((AvlNode *)bitree_data(grandchild))->factor) {
            case AVL_LFT_HEAVY:
                ((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
                ((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
                break;
            case AVL_BALANCED:
                ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
                break;
            case AVL_RGT_HEAVY:
                ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
                ((AvlNode *)bitree_data(right))->factor = AVL_LFT_HEAVY;
                break;
        }

        ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
        *node = grandchild;
    }
    return;
}

static void destroy_left(BisTree *tree, BiTreeNode *node)
{

}

void bistree_init(BisTree *tree, int (*compare)(const void *key1, const void *key2), void (*destroy)(void *data))
{

}

void bistree_destroy(BisTree *tree)
{
}

int bistree_insert(BisTree *tree, const void *data)
{
}

int bistree_remove(BisTree *tree, const void *data)
{
}

int bistree_lookup(BisTree *tree, void **data)
{
}
