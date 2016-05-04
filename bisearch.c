// 二分查找
#include <stdlib.h>
#include <string.h>

#include "search.h"

int bisearch(void *sorted, void *target, int size, int esize, int (*compare)(const void *key1, const void *key2))
{
    int left,
        middle,
        right;

    // 继续查找，直到左右两边索引交叉
    left = 0;
    right = size - 1;

    while (left <= right) {
        middle = (left + right) / 2;

        switch (compare(((char *)sorted + (esize * middle)), target)) {
            case -1:
                left = middle + 1;
                break;
            case 1:
                right = middle - 1;
                break;
            case 0:
                return middle;
        }
    }

    return -1;
}
