// 集合覆盖问题
#ifndef COVER_H
#define COVER_H

#include "set.h"

// 为子集的身份做一个key的数据结构
typedef struct Kset_
{
    void *key;
    Set *set;
} Kset;

int cover(Set *member, Set *subsets, Set *covering);

#endif
