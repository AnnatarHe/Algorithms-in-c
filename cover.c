#include <stdlib.h>
#include "cover.h"
#include "list.h"
#include "set.h"

int cover(Set *member, Set *subsets, Set *covering)
{
    Set intersection;
    KSet *subset;
    ListElmt *member,
             *max_member;
    void *data;
    int max_size;

    // 初始化覆盖的集合
    set_init(covering, subsets->match, NULL);

    // 没有覆盖的成员和候选子集后继续
    while (set_size(members) > 0 && set_size(subsets) > 0) {

        // 找到覆盖大多数成员的子集
        max_size = 0;

        for (member = list_head(subsets); member != NULL; member = list_next(member)) {
            if (set_intersection(&intersection, &((KSet *)list_data(member))->set, members) != 0) {
                return -1;
            }

            if (set_size(&intersection) > max_size) {
                max_member = member;
                max_size = set_size(&intersection);
            }

            set_destroy(&intersection);
        }

        // 没有交集的时候是不可能有覆盖的
        if (max_size == 0) {
            return 1;
        }

        subset = (KSet *)list_data(max_member);

        if (set_inter(covering, subset) != 0) {
            return -1;
        }

        // 在未被覆盖的成员的集合中移除每一个覆盖成员
        for (member = list_head(&((KSet *)list_data(max_member))->set); member != NULL; member = list_next(member)) {
            data = list_data(member);
            if (set_remove(members, (void**)&data) == 0 && members->destroy != NULL) {
                members->destroy(data);
            }
        }

        // 在候选子集中移除
        if (set_remove(subsets, (void **)&subset) != 0) {
            return -1;
        }
    }

    // 如果仍然有没有覆盖的成员, 没有覆盖的可能性
    if (set_size(members) > 0) {
        return -1;
    }

    return 0;
}
