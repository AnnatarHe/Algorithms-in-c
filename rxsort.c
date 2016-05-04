/**
 * 基数排序是一种高效的线性排序算法。
 *
 * 方法是，按位分开，并从数据的最低有效位到最高有效位进行比较，依次排序，从而得到有效数据集合
 */
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"

int rxsort(int *data, int size, int p, int k)
{
    int *counts,
        *temp;
    int index,
        pval,
        i,
        j,
        n;

    // 为计数器申请内存
    if ((counts = (int *)malloc(k * sizeof(int))) == NULL) {
        return -1;
    }

    // 为排序后的元素申请内存
    if ((temp = (int *)malloc(size * sizeof(int))) == NULL) {
        return -1;
    }

    for (n = 0; n < p; n++) {

        // 初始化计数器
        for (i = 0; i < k; i++) {
            counts[i] = 0;
        }

        // 计算那个位置的值
        pval = (int)pow((double)k, (double)n);

        // 记录下每个数字值出现的次数
        for (j = 0; j < size; j++) {
            index = (int)(data[j] / pval) % k;
            counts[index] = counts[index] + 1;
        }

        // 在反映计数之前调整每个计数
        for (i = 1; i < k; i++) {
            counts[i] = counts[i] + counts[i - 1];
        }

        // 使用计数器去确定每个元素应该呆的位置
        for (j = size - 1; j >= 0; j--) {
            index = (int)(data[j] / pval) % k;
            temp[counts[index] - 1] = data[j];
            counts[index] = counts[index] - 1;
        }

        memcpy(data, temp, size * sizeof(int));
    }
    free(counts);
    free(temp);
    
    return 0;
}
