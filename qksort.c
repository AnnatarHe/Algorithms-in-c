// 快排
// 快排的原理是不断的分割无序元素，直到所有分区都只剩单个元素
// 快排的关键在于如何分割数据，这部分工作由函数 partition 完成。函数分割data中处于 i 和 k 之间的元素
// 
// 首先用中位数发选一个中位数作为分割值，一旦选定分割值，就将 k 往 data 左边移动，直到找到一个小于或等于分割值的元素。这个元素属于左半分区。
// 接下来将 i 往右边移动，直到找到一个大于或等于分割值的元素。这个元素属于右分区
// 一旦找到的两个元素属于错误位置，就交换它们的位置。
// 重复这个过程直到 i 和 k 重合， 那么所有处于左边的元素都将小于它，右边的大于它。
//
// 现在看 qksort 如何处理递归， 在初次调用 qksort 时， i 设置为 0 ， k 设置为 size - 1
// 首先调用 partition 将 data 中处于 i 和 k 之间的元素分区。 当 partition 返回时把 j 赋予分割点的元素。
// 接下来递归调用 qksort 处理左边的分区(从 i 到 j), 直到 qksort 的分区只包含单个元素。 此时 i 不会比 k 小。递归中止
// 右分区同理
// 最终，数据完全排序完成
//
// 性能：
// 最坏情况    O(n^2)
// 平均性能    O(nlgn)
#include <stdlib.h>
#include <string.h>

#include "sort.h"

static int compare_int(const void *int1, const void *int2)
{
    // Compare two integers (used during median-of-three partitioning
    // 比较两个整型(在中位数的三个分区中使用)
    // From Google 机器翻译
    if (*(const int *)int1 > *(const int *)int2) {
        return 1;
    }else if (*(const int *)int1 < *(const int *)int2) {
        return -1;
    }else {
        return 0;
    }
}

static int partition(void *data, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    char *a = data;
    void *pval,
         *temp;
    int r[3];

    if ((pval = malloc(esize)) == NULL) {
        return -1;
    }

    if ((temp = malloc(esize)) == NULL) {
        free(pval);
        return -1;
    }

    // 使用中位数的方法找到分割值
    r[0] = (rand() % (k - i + 1)) + i;
    r[1] = (rand() % (k - i + 1)) + i;
    r[2] = (rand() % (k - i + 1)) + i;
    issort(r, 3, sizeof(int), compare_int);
    memcpy(pval, &a[r[1] * esize], esize);

    // 围绕分割值创建两个分割值
    i--;
    k++;

    while (1) {
        // 移动左边，直到在一个错误的分割值被找到
        do {
            k--;
        } while (compare(&a[k * esize], pval) > 0);

        do {
            i++;
        } while (compare(&a[i * esize], pval) < 0);

        if (i >= k) {
            // 当左右两边的计数器超过的时候，停止分割
            break;
        } else {
            // 交换两个元素，在左右两个计数器之下
            memcpy(temp, &a[i * esize], esize);
            memcpy(&a[i * esize], &a[k * esize], esize);
            memcpy(&a[k * esize], temp, esize);
        }
    }


    free(pval);
    free(temp);

    return k;
}

int qksort(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    int j;

    // 当不能再区分的时候，停止递归
    while ( i < k) {
        // 确定哪里去分割元素
        if ((j = partition(data, esize, i, k, compare)) < 0) {
            return -1;
        }

        // 递归排序左边的分割
        if (qksort(data, size, esize, i, j, compare) < 0) {
            return -1;
        }

        // 迭代并排序右边
        i = j + 1;
    }

    return 0;
}

