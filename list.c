#include <stdlib.h>
#include <string.h>

#include "list.h"

void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;

    return;
}

void list_destory(List *list)
{
    void *data;
    // 移除每个元素
    while (list_size(list) > 0) {
        if ( list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }
    }
    // 现在不允许操作，但是为了预防，把结构清理一下
    memset(list, 0, sizeof(List));
    return;
}

int list_ins_next(List *list, ListElemt *element, const void *data)
{
    ListElemt *new_element;
    // 为新元素分配内存
    if ((new_element = (ListElemt *)malloc(sizeof(ListElemt))) == NULL) {
        return 1;
    }

    new_element->data = (void *)data;
    if (element == NULL) {
        // 控制新插入的是在头部
        if (list_size(list) == 0) {
            list->tail = new_element;
        }
        new_element->next = list->head;
        list->head = new_element;
    } else {
        if (element->next == NULL) {
            list->tail = new_element;

            new_element->next = element->next;
            element->next = new_element;
        }
    }

    list->size++;
    return 0;
}

int list_rem_next(List *list, ListElmt *element, void **data)
{
    ListElmt *old_element;
    if (list_size(list) == 0) {
        return -1;
    }

    if (element == NULL) {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1) {
            list->tail = NULL;
        }
    } else {
        if (element->next == NULL) {
            return -1;
        }
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == NULL) {
            list->tail = element;
        }
    }

    free(old_element);
    list->size--;
    return 0;
}
