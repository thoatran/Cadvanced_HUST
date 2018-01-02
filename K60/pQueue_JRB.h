#include "jrb.h"
#include "dllist.h"
//#include <utility.h>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef JRB pQueue;

#define new_pQueue() \
			make_jrb()

#define is_empty_pQueue(Q) \
			jrb_empty(Q)

#define pQueue_insert(Q,k,v) \
			jrb_insert_int(Q,k,v)

#define pQueue_getMin(Q) \
			jrb_first(Q);

#define pQueue_deleteMin(Q) \
		jrb_delete_node(jrb_first(Q));

void decrease_key(pQueue Q, Jval v, int dv, int (*func)(Jval,Jval)){
	JRB i;
	jrb_traverse(i,Q){
		if(func(v, i->val) == 0){
			jrb_delete_node(i);
			break;
		}
	}

	pQueue_insert(Q,dv,v);
}
