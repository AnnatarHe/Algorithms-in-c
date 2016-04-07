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
                    // 第182页第5行
                    if ()
                }
            }
        }
    }
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
