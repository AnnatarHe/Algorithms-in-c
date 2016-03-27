#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "chtbl.h"

int chtbl_init(CHTbl *htbl, int buckets, int (*h)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
    int i;

    if ((htbl->table = (List *)malloc(buckets * sizeof(List))) === NULL) {
        return -1;
    }

    htbl->buckets = buckets;

    for (i = 0; i < htbl->buckets; i++) {
        list_init(&htbl->table[i], destroy);
    }

    // 封装功能
    htbl->h = h;
    htbl->match = match;
    htbl->destroy = destroy;

    htbl->size = 0;
    return 0;
}

int chtbl_destroy(CHTbl *htbl)
{
    int i;

    // 为每个桶执行析构函数
    for (i = 0; i < htbl->buckets; i++) {
        list_destory(&htbl->table(i));
    }

    free(htbl->table);

    memset(htbl, 0, sizeof(CHTbl));

    return;
}

int chtbl_insert(CHTbl *htbl, const void *data)
{
    void *temp;
    int bucket,
        retval;

    // 如果数据已经存在在table中就不做任何事
    temp = (void *)data;
    if (chtbl_lookup(htbl, &temp)) {
        return 1;
    }

    bucket = htbl->h(data) % htbl->buckets;

    // 在桶中插入数据
    if ((retval = list_ins_next(&htbl->table[bucket], NULL, data)) == 0) {
        htbl->size++;
    }

    return retval;
}

int chtbl_remove(CHTbl *htbl, void **data)
{
    ListElmt *element,
             *prev;
    int bucket;

    bucket = htbl->h(*data) % htbl->buckets;

    prev = NULL;

    for (element = list_head(&htbl->table[bucket]); element != NULL; element = list_next(element)) {
        if (htbl->match(*data, list_data(element))) {
            // 从桶中移除数据
            if (list_rem_next(&htbl->table[bucket], prev, data) == 0) {
                htbl->size--;
                return 0;
            }else {
                return -1;
            }
        }

        prev = element;
    }

    // 返回没有找到数据
    return -1;
}

int chtbl_lookup(const CHTbl *htbl, void **data)
{
    ListElmt *element;
    int bucket;

    bucket = htbl->h(*data) % htbl->buckets;

    for (element = list_head(&htbl->table[bucket]); element != NULL; element = list_next(element)) {
        if (htbl->match(*data, list_data(element))) {
            // 从table中传回数据
            *data = list_data(element);
            return 0;
        }
    }

    return -1;
}
