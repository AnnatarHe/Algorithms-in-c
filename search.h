#ifndef SEARCH_H
#define SEARCH_H
// 二分查找
int bisearch(void *sorted, void *target, int size, int esize, int (*compare)(const void *key1, const void *key2));

#endif
