#include <stdlib.h>

#include "list.h"
#include "queue.h"

int queue_enqueue(Queue *queue, const void *data)
{
    return list_ins_next(queue, list_tail(queue), data);
}
int queue_dequeue(Queue *quque, void **data)
{
    return list_rem_next(queue, NULL, data);
}
