#ifndef SORT_H
#define SORT_H

/**
 * @param data 元素
 * @param size 元素的size
 * @param esize 每个元素的大小
 * @param compare 比较函数指针 返回0, -1, 1
 */
int issort(void *data, int size, int esize, int (*compare)(const void *key1, const void *key2));

#endif
