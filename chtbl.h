// 链式哈希表
// 包含一组"桶"，每个"桶"是一个链表
#ifndef CHTBL_H
#define CHTBL_H

#include <stdlib.h>

#include "list.h"

typedef struct CHTbl_
{
    int buckets;

    int (*h)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    int size;
    List *table;
} CHTbl;

/**
 * @params *htbl 初始化的hash table
 * @params buckets 桶的个数
 * @params h, match destroy 传入其中的函数
 * @params size 表中现在存在的元素数量
 * @params table 存储桶的数组
 */
int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *data));

int chtbl_destroy(CHTbl *htbl);

int chtbl_insert(CHTbl *htbl, const void *data);

int chtbl_remove(CHTbl *htbl, void **data);

int chtbl_lookup(const CHTbl *htbl, void **data);

#define chtbl_size(htbl) ((htbl)->size)

#endif

