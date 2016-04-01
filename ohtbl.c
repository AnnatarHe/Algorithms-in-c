#include <stdlib.h>
#include <string.h>

#include "ohtbl.h"

static char vacated;

int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key), int (*h2)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
    int i;

    if ((htbl->table = (void **)malloc(positions * sizeof(void *))) == NULL) {
        return -1;
    }

    htbl->positions = positions;
    // 初始化每一个位置
    for (i = 0; i < htbl->positions; i++) {
        htbl->table[i] = NULL;
    }
    htbl->vacated = &vacated;

    htbl->h1 = h1;
    htbl->h2 = h2;
    htbl->match = match;
    htbl->destroy = destroy;

    htbl->size = 0;
    return 0;
}

void ohtbl_destroy(OHTbl *htbl, const void *data)
{
    void *temp;
    int position,
        i;

    if (htbl->size == htbl->positions) {
        return -1;
    }

    temp = (void *)data;
    if (ohtbl_lookup(htbl, &temp) == 0) {
        return 1;
    }

    // 用两重哈希来哈希这个键
    for (i = 0; i < htbl->positions; i++) {
        position = (htbl->h1(data) + (i * htbl->h2(data))) % htbl->positions;

        if (htbl->table[position] == NULL || htbl->table[position] == htbl->vacated) {
            // 在表中插入数据
            htbl->table[position] = (void *)data;
            htbl->size++;
            return 0;
        }
    }

    return -1;
}

int ohtbl_insert(OHTbl *htbl, void **data)
{
    int position,
        i;

    for (i = 0; i < htbl->positions; i++) {
        position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
        if (htbl->table[position] == NULL) {
            // 返回未找到
            return -1;
        }else if (htbl->table[position] == htbl->vacated) {
            // 在放弃的位置前搜索
            continue;
        }else if (htbl->match(htbl->table[position], *data)) {
            // 从table中传回数据
            *data = htbl->table[position];
            htbl->table[position] = htbl->vacated;
            htbl->size--;
            return 0;
        }
    }

    return -1;
}

int ohtbl_remove(OHTbl *htbl, void **data)
{
    int position,
        i;
    // 使用两个哈希来哈希key
    for (i = 0; i < htbl->positions; i++) {
        position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
        if (htbl->table[position] == NULL) {
            // not found
            return -1;
        }else if (htbl->table[position] == htbl->vacated) {
            // 在放弃的位置之前搜寻数据
            continue;
        }else if (htbl->match(htbl->table[position], *data)) {
            // 从table中传回数据
            *data = htbl->table[position];
            htbl->table[position] = htbl->vacated;
            htbl->size--;
            return 0;
        }
    }

    return -1;
}

int ohtbl_lookup(const OHTbl *htbl, void **data)
{
    int position,
        i;
    // 使用两个哈希来哈希key
    for (i = 0; i < htbl->positions; i++) {
        position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;
        if (htbl->table[position] == NULL) {
            // not found
            return -1;
        }else if (htbl->match(htbl->table[position], *data)) {
            // 从table中传回数据
            *data = htbl->table[position];
            return 0;
        }
    }
    return -1;
}
