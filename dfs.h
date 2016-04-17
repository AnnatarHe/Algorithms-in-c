// 深度优先搜索头文件
#ifndef DFS_H
#define DFS_H

#include "graph.h"
#include "list.h"

typedef struct DfsVertex_
{
    void *data;
    VertexColor color;
} DfsVertex;

int dfs(Graph *graph, List *ordered);

#endif
