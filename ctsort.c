/**
 * 计数排序，高效的线性排序
 * 通过计算一个集合中元素出现的次数来确定集合如何排列。
 * 计数排序不需要进行元素比较。而且它的效率比 O(nlgn) 比较排序高
 *
 * 局限性：只能是整型的。还需要知道集合中最大整数的值，以便为数组分配空间
 *
 * 除了速度快以外，另一个优点是稳定
 */
#include <stdlib.h>
#include <string.h>
#include "sort.h"
int ctsort(int *data, int size, int k)
{

    int *counts,
        *temp;
    int i,
        j;

    if ((counts = (int *)malloc(k * sizeof(int))) == NULL) {
        return -1;
    }

    if ((temp = (int *)malloc(k * sizeof(int))) == NULL) {
        return -1;
    }

    // 初始化计数器
    for (i = 0; i < k; i++) {
        counts[i] = 0;
    }

    // 为每一个元素计数
    for (j = 0; j < size; j++) {
        counts[data[j]] = counts[data[j]] + 1;
    }

    // 在之前，调整每个计数去反映到总数上
    for (i = 1; i < k; i++) {
        counts[i] = counts[i] + counts[i - 1];
    }

    // 使用计数去定位每个元素所属的位置
    for (j = size - 1; j >= 0; j--) {
        temp[counts[data[j]] - 1] = data[j];
        counts[data[j]] = counts[data[j]] - 1;
    }

    // 准备返回排序好的数据
    memcpy(data, temp, size * sizeof(int));

    free(counts);
    free(temp);

    return 0;
}
