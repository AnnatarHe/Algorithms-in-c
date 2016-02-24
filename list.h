#ifndef LIST_H
#define LIST_H

/**
 * 链表头文件
 */
#include <stdlib.h>
// 链表单个元素
typedef struct ListElemt_
{
    void *data;
    struct ListElemt_ *next;
} ListElemt;

// 链表
typedef struct List_
{
    // 元素个数
    int size;
    // 并不由链表本身使用，而是从链表数据结构派生而来的新类型所使用
    int (*match)(const void *key1, const void *key2);
    // 封装后传递给list_init用的析构函数
    void (*destroy)(void *data);
    // 头指针
    ListElemt *head;
    // 尾指针
    ListElemt *tail;
} List;

void list_init(List *list, void (*destroy)(void *data));
void list_destory(List *list);
int list_ins_next(List *list, ListElemt *element, const void *data);
int list_rem_next(List *list, ListElemt *element, void **data);
#define list_size(list) ((list)->size)

#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#endif

