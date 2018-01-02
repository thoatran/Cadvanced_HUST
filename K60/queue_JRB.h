#include "jrb.h"
#include "dllist.h"
//../../lib/include/libfdr/jrb.h
//#include <utility.h>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define new_queue() \
			new_dllist()
			
#define enqueue(Q,j) \
			dll_append(Q,j)

#define free_queue(Q) \
			free_dllist(Q)

#define queue_empty(Q) \
			dll_empty(Q)

typedef Dllist QueueNode;
typedef QueueNode Queue;

Jval dequeue(Queue Q){
	QueueNode first = dll_first(Q);
	Jval v = first->val;
	dll_delete_node(first);
	return v;
}
