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
    BiTreeNode **position;

    if (bitree_size(tree) == 0) {
        return;
    }

    if (node == NULL) {
        position = &tree->root;
    }else {
        position = &tree->left;
    }

    // 删除节点
    if (*position != NULL) {
        destroy_left(tree, *position);
        destroy_right(tree, *position);

        if (tree->destroy != NULL) {
            // 调用用户自定的销毁函数去释放动态申请的数据
            tree->destroy(((AvlNode *)(*position)->data)->data);
        }

        // 先释放AVL里面的数据，然后释放节点自身
        free((*position)->data);
        free(*position);
        *position = NULL;

        tree->size--;
    }

    return;
}

static void destroy_right(BisTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;

    if (bitree_size(tree) == 0) {
        return;
    }

    if (node == NULL) {
        position = &tree->root;
    }else {
        position = &tree->right;
    }

    // 删除节点
    if (*position != NULL) {
        destroy_left(tree, *position);
        destroy_right(tree, *position);

        if (tree->destroy != NULL) {
            // 调用用户自定的销毁函数去释放动态申请的数据
            tree->destroy(((AvlNode *)(*position)->data)->data);
        }

        // 先释放AVL里面的数据，然后释放节点自身
        free((*position)->data);
        free(*position);
        *position = NULL;

        tree->size--;
    }

    return;
}

static int insert(BisTree *tree, BiTreeNode **node, const void *data, int *balanced)
{
    AvlNode *avl_data;
    int cmpval, retval;

    // 把数据插入到树中
    if (bitree_is_eob(*node)) {
        // 控制将其插入到空树中
        if ((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL) {
            return -1;
        }

        avl_data->factor = AVL_BALANCED;
        avl_data->hidden = 0;
        avl_data->data = (void *)data;

        return bitree_ins_left(tree, *node, avl_data);
    }else {

        cmpval = tree->compare(data, ((AvlNode *)bitree_data(*node))->data);
        if (cmpval < 0) {
            if (bitree_is_eob(bitree_left(*node))) {
                if ((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL) {
                    return -1;
                }

                avl_data->factor = AVL_BALANCED;
                avl_data->hidden = 0;
                avl_data->data = (void *)data;

                if (bitree_ins_left(tree, *node, avl_data) != 0) {
                    return -1;
                }

                *balanced = 0;
            }else {
                if ((retval = insert(tree, &bitree_left(*node), data, balanced)) != 0) {
                    return retval;
                }
            }

            if (! (*balanced)) {
                switch (((AvlNode *)bitree_data(*node))->factor) {
                    case AVL_LFT_HEAVY:
                        rotate_left(node);
                        *balanced = 1;
                        break;
                    case AVL_BALANCED:
                        ((AvlNode *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
                        break;
                    case AVL_RGT_HEAVY:
                        ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
                        *balanced = 1;
                        break;
                }
            }
        }else if (cmpval > 0) { /* if (cmpval < 0)*/
            if (bitree_is_eob(bitree_right(*node))) {
                if ((avl_data = (AvlNode *)malloc(sizeof(AvlNode))) == NULL) {
                    return -1;
                }

                avl_data->factor = AVL_BALANCED;
                avl_data->hidden = 0;
                avl_data->data = (void *)data;

                if (bitree_ins_right(tree, *node, avl_data) != 0) {
                    return -1;
                    *balanced = 0;
                }else {
                    if ((retval = insert(tree, &bitree_right(*node), data, balanced)) != 0) {
                        return retval;
                    }
                }

                // 确保树仍然是平衡的
                if (!(*balanced)) {
                    switch (((AvlNode *)bitree_data(*node))->factor) {
                        case AVL_LFT_HEAVY:
                            ((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
                            *balanced = 1;
                            break;
                        case AVL_BALANCED:
                            ((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
                            break;
                        case AVL_RGT_HEAVY:
                            rotate_right(node);
                            *balanced = 1;
                    }
                }
            } // if (cmpval > 0)
            else {
                // 操控找到数据的复制品
                if (! ((AvlNode *)bitree_data(*node))->hidden) {
                    // 数据在树中，并且不隐藏就不做任何事情
                    return 1;
                }else {
                    // 插入一个数据，并且将其标记为不隐藏
                    if (tree->destroy != NULL) {
                        // 从开始替换的地方删掉隐藏元素
                        tree->destroy(((AvlNode *)bitree_data(*node))->data);
                    }
                    ((AvlNode *)bitree_data(*node))->data = (void *)data;
                    ((AvlNode *)bitree_data(*node))->hidden = 0;

                    // 不能更改平衡性，因为树的结构并没有发生改变
                    *balanced = 1;
                }
            }
        } // 这里多了一个括号，不知道是哪里多的，再查
        return 0;
    }
}

static int hide(BisTree *tree, BiTreeNode *node, const void *data)
{
    int cmpval, retval;
    if (bistree_is_eob(node)) {
        // 没找到就返回
        return -1;
    }
    cmpval = tree->compare(data, ((AvlNode *)bitree_data(node))->data);
    if (cmpval < 0) {
        // 移到左节点
        retval = hide(tree, bitree_left(node), data)
    }else if (cmpval > 0) {
        retval = hide(tree, bitree_right(node), data)
    }else {
        // 把这个节点隐藏
        ((AvlNode *)bitree_data(node))->hidden = 1;
        retval = 0;
    }
    return retval;
}

static int lookup(BisTree *tree, BiTreeNode *node, void **data)
{
    int cmpval, retval;
    if (bistree_is_eob(node)) {
        // 没找到就返回
        return -1;
    }
    cmpval = tree->compare(data, ((AvlNode *)bitree_data(node))->data);
    if (cmpval < 0) {
        // 移到左节点
        retval = lookup(tree, bitree_left(node), data)
    }else if (cmpval > 0) {
        retval = lookup(tree, bitree_right(node), data)
    }else {
        if (! ((AvlNode *)bitree_data(node))->hidden) {
            *data = ((AvlNode *)bitree_data(node))->data;
            retval = 0;
        }else {
            // 404
            return -1;
        }
    }
    return retval;
}

void bistree_init(BisTree *tree, int (*compare)(const void *key1, const void *key2), void (*destroy)(void *data))
{
    bitree_init(tree, destroy);
    tree->compare = compare;

    return;
}

void bistree_destroy(BisTree *tree)
{
    destroy_left(tree, NULL);

    memset(tree, 0, sizeof(BisTree));
    return;
}

int bistree_insert(BisTree *tree, const void *data)
{
    int balanced = 0;
    return insert(tree, &bitree_root(tree), data, &balanced);
}

int bistree_remove(BisTree *tree, const void *data)
{
    return hide(tree, bitree_root(tree), data);
}

int bistree_lookup(BisTree *tree, void **data)
{
    return lookup(tree, bitree_root(tree), data);
}
