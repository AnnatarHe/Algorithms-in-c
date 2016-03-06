/**
 * 第二次机会页面置换算法
 *
 * 循环链表 的应用
 */
#ifndef PAGE_H
#define PAGE_H

#include "clist.h"

typedef struct Page_
{
    int number;
    int reference;
} Page;

int replace_page(CListElmt **current);

#endif
