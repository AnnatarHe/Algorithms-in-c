#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "set.h"

void set_init(Set *set, int (*match)(const void *key1, const void *key2), void (*destory)(void *data))
{
    list_init(set, destory);
    set->match = match;
    return;
}
int set_insert(Set *set, const void *data)
{
    if (set_is_member(set, data)) {
        return 1;
    }
    return list_ins_next(set, list_tail(set), data);
}
int set_remove(Set *set, void **data)
{
    ListElmt *member, *prev;
    // 找到需要移除的元素
    prev = NULL;

    for(member = list_head(set); member != NULL; member = list_next(member)) {
        if (set->match(*data, list_data(member))) {
            break;
        }
        prev = member;
    }

    // 如果没有找到这个成员就返回
    if (member == NULL) {
        return -1;
    }

    // 移除操作
    return list_rem_next(set, prev, data);
}
int set_union(Set *setu, const Set *set1, const Set *set2)
{
    ListElmt *member;
    void *data;
    // 初始化并集
    set_init(setu, set1->match, NULL);
    
    // 为第一个set添加成员
    for( member = list_head(set1); member != NULL; member = list_next(member)) {
        data = list_data(member);
        if (list_ins_next(setu, list_tail(setu), data) != 0) {
            set_destroy(setu);
            return -1;
        }
    }
    // 为第二个set添加成员
    for( member = list_head(set2); member != NULL; member = list_next(member)) {
        if (set_is_member(set1, list_data(member))) {
            // 防止重复插入
            continue;
        }else {
            data = list_data(member);
            if (list_ins_next(setu, list_tail(setu), data) != 0) {
                set_destroy(setu);
                return -1;
            }
        }
    }
    return 0;
}
int set_intersection(Set *seti, const Set *set1, const Set *set2)
{
    ListElmt *member;
    void *data;
    // 初始化交集
    set_init(seti, set1->match, NULL);

    // 插入两个集合的值
    for( member = list_head(set1); member != NULL; member = list_next(member)) {
        if (set_is_member(set2, list_data(member))) {
            data = list_data(member);
            if (list_ins_next(seti, list_tail(seti), data) != 0) {
                set_destroy(seti);
                return -1;
            }
        }
    }
    return 0;
}
int set_difference(Set *setd, const Set *set1, const Set set2)
{
    ListElmt *member;
    void *data;
    // 初始化diff集
    set_init(setd, set1->match, NULL);

    // 插入来自于set1而set2并不存在的值
    for(member = list_head(set1); member != NULL; member = list_next(member)) {
        if ( ! set_is_member(set2, list_data(member))) {
            data = list_data(member);
            if (list_ins_next(setd, list_tail(setd), data) != 0) {
                set_destroy(setd);
                return -1;
            }
        }
    }
    return 0;
}
int set_is_member(const Set *set, const void *data)
{
    ListElmt *member;

    // 确认此值是不是这个集合的成员
    for (member = list_head(set); member != NULL; member = list_next(member)) {
        if (set->match(data, list_data(member))) {
            return 1;
        }
    }

    return 0;
}
int set_is_subset(const Set *set1, const Set *set2)
{
    ListElmt *member;

    // 做一个快速的测试，排除一些情况
    if (set_size(set1) > set_size(set2)) {
        return 0;
    }

    // 确认set1是set2的子集
    for (member = list_head(set1); member != NULL; member = list_next(set1)) {
        if ( ! set_is_member(set2, list_data(member))) {
            return 0;
        }
    }
    return 1;
}
int set_is_equal(const Set *set1, const Set *set2)
{
    // 做一个快速的测试，排除一些情况
    if (set_size(set1) != set_size(set2)) {
        return 0;
    }

    return set_is_subset(set1, set2);
}
