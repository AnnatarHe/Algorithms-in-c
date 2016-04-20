#ifndef SORT_H
#define SORT_H

/**
 * @param data 元素
 * @param size 元素的size
 * @param esize 每个元素的大小
 * @param compare 比较函数指针 返回0, -1, 1
 */
int issort(void *data, int size, int esize, int (*compare)(const void *key1, const void *key2));

/**
 * @param i, k 定义当前进行排序的两个部分, 其值分别初始化为 0, size - 1
 * @return 当返回时，data包含已排序的元素
 */
int qksort(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2));

/**
 * 归并排序
 */
int mgsort(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2));

/**
 * 计数排序
 */
int ctsort(int *data, int size, int k);

/**
 * 基数排序
 */
int rxsort(int *data, int size, int p, int k);

#endif
