#include "jrb.h"
#include "dllist.h"
//#include <utility.h>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_JRB.h"
#include "pQueue_JRB.h"

#define MAX_ID_SIZE 1000
#define WHITE 0
#define GREY 1
#define BLACK 2
#define INF 2000000

typedef struct {
	JRB edges;
	JRB vertices;
	int (*func)(Jval,Jval);
} Graph;

void setVertexID(Jval *v){
	int id;
	scanf("%d%*c",&id);
	*v = new_jval_i(id);
}

void visitNode(JRB n){
	printf("\nMa san pham: %d\nTen san pham: %s\n", jval_i(n->key), jval_s(n->val));
}

/*----------------------CAC THAO TAC TREN GRAPH---------------------------------*/

Graph createGraph(int (*func)(Jval,Jval)); //---------------------- khoi tao do thi moi
int addVertex(Graph graph, Jval id, Jval name); //----------------- them 1 dinh
int addEdge(Graph graph, Jval v1, Jval v2, int weight); //--------- them 1 canh
int hasEdge(Graph graph, Jval v1, Jval v2); //--------------------- kiem tra canh giua 2 dinh
int getGraphSize(Graph graph); // --------------------------------- lay so dinh cua do thi
int outdegree(Graph graph, Jval v, JRB output); // ---------------- so canh ra tu dinh v
int indegree(Graph graph, Jval v, JRB output); // ----------------- so canh di vao dinh v

// BFS option:
// 1 - Duyet tat ca dinh co the toi dc tu s
// 2 - Duyet tat ca cac dinh trong do thi
// 3 - Khoang cach tu s toi 1 dinh trong do thi (theo so dinh di qua)
// 4 - Dinh lien truoc khi duyet tu s
void BFS(Graph graph, Jval s, void (*visitNode)(JRB), int option);

// DFS option:
// 1 - Duyet tat ca dinh co the toi dc tu s
// 2 - Duyet tat ca cac dinh trong do thi
// 3 - Thu tu duyet (bat dau / ket thuc duyet 1 nut)
// 4 - Dinh lien truoc khi duyet tu s
void DFS(Graph graph, Jval s, void (*visitNode)(JRB), int option);


int hasCycle(Graph graph, int option); // ------------------------- kiem tra chu trinh, option == 1: do thi co huong
void topo(Graph graph, Jval s); // -------------------------------- sap xep topo

/* option == 1: in duong di */ 
int Dijkstra(Graph graph, Jval v1, Jval v2, int option); // ------- tim duong di ngan nhat-do thi ko co trong so am
int SPFA(Graph graph, Jval v1, Jval v2, int option); // ----------- tim duong di ngan nhat-do thi co trong so am

void dropGraph(Graph graph); // ----------------------------------- xoa toan bo do thi


Graph createGraph(int (*func)(Jval,Jval)){
	Graph graph;
	graph.edges = make_jrb();
	graph.vertices = make_jrb();
	graph.func = func;
	return graph;
}

int addVertex(Graph graph, Jval id, Jval name){
	JRB node = jrb_find_gen(graph.vertices, id, graph.func);
	if(node != NULL){
		// printf("Vertex already exits\n");
		return 0;
	}
	else {
		JRB tree = make_jrb();
		jrb_insert_gen(graph.edges, id, new_jval_v(tree), graph.func);
		jrb_insert_gen(graph.vertices, id, name, graph.func);
		return 1;
	}
}

int addEdge(Graph graph, Jval v1, Jval v2, int weight){
	JRB tree;
	JRB node1 = jrb_find_gen(graph.edges, v1, graph.func);
	JRB node2 = jrb_find_gen(graph.edges, v2, graph.func);
	int w;

	if(node1 != NULL && node2 != NULL){
		tree = (JRB) jval_v(node1->val);
		JRB v2e = jrb_find_gen(tree, v2, graph.func);
		if(v2e == NULL)
			jrb_insert_gen(tree, v2, new_jval_i(weight), graph.func);
		else {
			w = jval_i(v2e->val);
			w++;
			v2e->val = new_jval_i(w);
		}

		return 1;
	}
	else {
		printf("Vertex(s) doesn't exits.\nAdd vertex first.\n\n");
		return 0;
	}
}

int hasEdge(Graph graph, Jval v1, Jval v2){
	JRB tree;
	JRB node1 = jrb_find_gen(graph.edges, v1, graph.func);
	JRB node2 = jrb_find_gen(graph.edges, v2, graph.func);

	if(node1 != NULL && node2 != NULL){
		tree = (JRB) jval_v(node1->val);
		JRB v2e = jrb_find_gen(tree, v2, graph.func);
		if(v2e == NULL)
			return -1;
		else 
			return jval_i(v2e->val);
	}
	else {
		// printf("Vertex %d or %d doesn't exits.\nAdd vertex first.\n\n", v1, v2);
		return -1;
	}
}

int getGraphSize(Graph graph){
	int graph_size = 0;
	JRB node;
	jrb_traverse(node, graph.vertices){
		graph_size++;
	}
	return graph_size;
}

int outdegree(Graph graph, Jval v, JRB output){
	JRB node = jrb_find_gen(graph.edges, v, graph.func);
	if(node == NULL){
		// printf("Graph doesn't have node %d\n", v);
		return -1;
	}
	else {
		JRB tree = (JRB) jval_v(node->val);
		JRB n;

		int total = 0;
		jrb_traverse(n, tree){
			jrb_insert_gen(output, n->key, new_jval_v(NULL), graph.func);
			total++;
		}
		return total;
	}
}

int indegree(Graph graph, Jval v, JRB output){
	JRB inNode = jrb_find_gen(graph.vertices, v, graph.func);
	if(inNode == NULL){
		// printf("Graph doesn't have node %d\n", v);
		return 0;
	}
	else {
		int total = 0;

		JRB toNode, fromNode, tree;

		jrb_traverse(fromNode, graph.edges){
			tree = (JRB)jval_v(fromNode->val);
			jrb_traverse(toNode, tree){
				if(graph.func(toNode->key, v) == 0){
					jrb_insert_gen(output, fromNode->key, new_jval_v(NULL), graph.func);
					total++;
				}
			}
		}
		return total;
	}
}

void printPathBFS(Graph graph, JRB p, Jval f, Jval t, void (*visitNode)(JRB)){
	if(graph.func(f, t) == 0)
		visitNode(jrb_find_gen(graph.vertices, f, graph.func));
	else {
		JRB node;
		Jval prev;

		jrb_traverse(node, p){
			if(graph.func(node->key, t) == 0)
				prev = node->val;
			
		}
		printPathBFS(graph, p, f , prev, visitNode);
		visitNode(jrb_find_gen(graph.vertices, t, graph.func));
	}
}

void BFS_Visit(Graph graph, Jval s, JRB color, JRB dis, JRB prev, void (*visitNode)(JRB)) {
	if(jrb_find_gen(graph.edges, s, graph.func) == NULL) 
		return;
	else {
		JRB u, tree, v, snode, c, d, p;
		int n;

		Queue Q = new_queue();

		enqueue(Q,s);

		c = jrb_find_gen(color, s, graph.func);
		c->val = new_jval_i(GREY);

		while(!queue_empty(Q)) {
			u = jrb_find_gen(graph.edges, dequeue(Q), graph.func);
			if(visitNode != NULL) 
				visitNode(jrb_find_gen(graph.vertices, u->key, graph.func));

			tree = (JRB)jval_v(u->val);

			jrb_traverse(v,tree) {
				c = jrb_find_gen(color, v->key, graph.func);
				d = jrb_find_gen(dis, v->key, graph.func);
				p = jrb_find_gen(prev, v->key, graph.func);

				if(jval_i(c->val) == WHITE) {			
					c->val = new_jval_i(GREY);				
					dis->val = new_jval_i(jval_i(dis->val) + 1);
					p->val = u->key;
					enqueue(Q, v->key);
				}
			}

			c = jrb_find_gen(color, u->key, graph.func);
			c->val = new_jval_i(BLACK);

		}

		free_queue(Q);
		printf("\n");
	}
}

void printNodeVal(JRB node){
	printf("%s - ", jval_s(node->val));
}

void BFS(Graph graph, Jval s, void (*visitNode)(JRB), int option){
	JRB color = make_jrb();
	JRB dis = make_jrb();
	JRB prev = make_jrb();
	JRB node, c, p;

	jrb_traverse(node, graph.vertices){
		jrb_insert_gen(color, node->key, new_jval_i(WHITE), graph.func);
		jrb_insert_gen(dis, node->key, new_jval_i(INF), graph.func);
		jrb_insert_gen(prev, node->key, new_jval_v(NULL), graph.func);
	}

	node = jrb_find_gen(dis, s, graph.func);
	node->val = new_jval_i(0);

	Jval i = s, v, u;
	int t = 1;
	
	switch(option){
		case 1:
			BFS_Visit(graph, s, color, dis, prev, visitNode);
			break;

		case 2:
			printf("Component 1\n");
			BFS_Visit(graph, s, color, dis, prev, visitNode);

			jrb_traverse(node,graph.vertices){
				c = jrb_find_gen(color, node->key, graph.func);

				if(jval_i(c->val) == WHITE){
					printf("Component %d\n", ++t);
					BFS_Visit(graph, node->key, color, dis, prev, visitNode);
					printf("\n");
				}
			}
			break;

		case 3:
			printf("Vertex ID: ");
			setVertexID(&v);
			if(jrb_find_gen(graph.vertices, v, graph.func) == NULL) 
				printf("Vertex doesn't exits\n");
			else {
				BFS_Visit(graph, s, color, dis, prev, visitNode);
				printf("The distance between s and v is: ");
				printf("%d\n", jval_i(jrb_find_gen(dis, v, graph.func)->val));
			}
			break;

		case 4:
			printf("Nhap ma san pham 2: "); 
			setVertexID(&v);
			if(jrb_find_gen(graph.vertices, s, graph.func) == NULL || jrb_find_gen(graph.vertices, v, graph.func) == NULL) 
				printf("Ma so khong ton tai\n");
			else {
				BFS_Visit(graph, s, color, dis, prev, NULL);

				p = jrb_find_gen(prev, v, graph.func);

				if(jval_v(p->val) != NULL) {
					printPathBFS(graph, prev, s, v, visitNode);
					printf("\n");
				}
				else 
					printf("Hai san pham khong co lien he voi nhau\n");
			}
			break;
	}

	jrb_free_tree(color);
	jrb_free_tree(dis);
	jrb_free_tree(prev);

}

void DFS_Visit(Graph graph, Jval u, JRB color, JRB d, JRB f, JRB prev, void (*visitNode)(JRB), int *t){
	if(jrb_find_gen(graph.vertices, u, graph.func) == NULL) 
		return;
	else {
		visitNode(jrb_find_gen(graph.vertices, u, graph.func));

		JRB node, c, begin, finish, p, v;

		c = jrb_find_gen(color, u, graph.func);
		c->val = new_jval_i(GREY);

		(*t)++;

		begin = jrb_find_gen(d, u, graph.func);
		begin->val = new_jval_i(*t);

		JRB output = make_jrb();

		int n = outdegree(graph, u, output);

		jrb_traverse(v, output){
			c = jrb_find_gen(color, v->key, graph.func);
			p = jrb_find_gen(prev, v->key, graph.func);

			if(jval_i(c->val) == WHITE){
				p->val = u;
				DFS_Visit(graph, v->key, color, d, f, prev, visitNode, t);
			}
		}

		c = jrb_find_gen(color, u, graph.func);
		c->val = new_jval_i(BLACK);

		(*t)++;

		finish = jrb_find_gen(f, u, graph.func);
		finish->val = new_jval_i(*t);

		jrb_free_tree(output);
	}
}

void DFS(Graph graph, Jval s, void (*visitNode)(JRB), int option){
	JRB color = make_jrb();
	JRB d = make_jrb();
	JRB f = make_jrb();
	JRB prev = make_jrb();
	JRB node, c, p;

	jrb_traverse(node, graph.vertices){
		jrb_insert_gen(color, node->key, new_jval_i(WHITE), graph.func);
		jrb_insert_gen(d, node->key, new_jval_i(0), graph.func);
		jrb_insert_gen(f, node->key, new_jval_i(0), graph.func);
		jrb_insert_gen(prev, node->key, new_jval_v(NULL), graph.func);
	}

	Jval dval, fval, v;
	int t = 0, time = 1;

	switch(option){
		case 1:
			DFS_Visit(graph, s, color, d, f, prev, visitNode, &t);
			break;
		case 2:
			printf("Component 1\n");
			DFS_Visit(graph, s, color, d, f, prev, visitNode, &t);

			jrb_traverse(node, graph.vertices){
				c = jrb_find_gen(color, node->key, graph.func);
				if(jval_i(c->val) == WHITE){
					printf("Component %d\n", ++time);
					DFS_Visit(graph, node->key, color, d, f, prev, visitNode, &t);
					printf("\n");
				}
			}
			
			break;

		case 3:
			DFS_Visit(graph, s, color, d, f, prev, visitNode, &t);
			jrb_traverse(node, graph.vertices){
				c = jrb_find_gen(color, node->key, graph.func);
				if(jval_i(c->val) == WHITE){
					DFS_Visit(graph, node->key, color, d, f, prev, visitNode, &t);
					printf("\n");
				}
			}
			printf("%-5s%-10s%-15s\n","ID","Name","Timestamps");

			jrb_traverse(node, graph.vertices){
				dval = jrb_find_gen(d, node->key, graph.func)->val;
				fval = jrb_find_gen(f, node->key, graph.func)->val;
				printf("%-5d%-10s%7d/%-7d\n", jval_i(node->key), jval_s(node->val), jval_i(dval), jval_i(fval));
			}

			break;

		case 4:
			printf("Vertex ID: "); 
			setVertexID(&v);
			if(jrb_find_gen(graph.edges, v, graph.func) == NULL) 
				printf("Vertex doesn't exits\n");
			else {
				DFS_Visit(graph, s, color, d, f, prev, visitNode, &t);
				p = jrb_find_gen(prev, v, graph.func);
				if(p != NULL) {
					printf("The previous vertex is: ");
					visitNode(jrb_find_gen(graph.vertices, p->val, graph.func));
				}
				else 
					printf("Vertex has no indegree when traverse from start\n");
			}
			break;
	}

	jrb_free_tree(color);
	jrb_free_tree(d);
	jrb_free_tree(f);
	jrb_free_tree(prev);
}

// option == 1 : do thi co huong
int detectingCycle(Graph graph, Jval u, JRB color, Jval prev, int option){
	JRB output = make_jrb();
	JRB c, node;

	c = jrb_find_gen(color, u, graph.func);
	c->val = new_jval_i(GREY);

	int n = outdegree(graph, u, output);

	jrb_traverse(node, output){
		if(graph.func(node->key, prev) != 0 || option == 1){
			c = jrb_find_gen(color, node->key, graph.func);
			if(jval_i(c->val) == GREY)
				return 1;
			else if(jval_i(c->val) == WHITE && detectingCycle(graph, node->key, color, u, option))
				return 1;
		}
	}

	c = jrb_find_gen(color, u, graph.func);
	c->val = new_jval_i(BLACK);

	jrb_free_tree(output);

	return 0;
}

int hasCycle(Graph graph, int option){
	JRB color = make_jrb();
	JRB node, c;

	jrb_traverse(node, graph.vertices){
		jrb_insert_gen(color, node->key, new_jval_i(WHITE), graph.func);
	}

	jrb_traverse(node, graph.vertices){
		c = jrb_find_gen(color, node->key, graph.func);
		if(jval_i(c->val) == WHITE)
			if(detectingCycle(graph, c->key, color, new_jval_v(NULL), 0)) return 1;
	}

	jrb_free_tree(color);
	return 0;
}


void topo_Visit(Graph graph, Jval u, JRB color, Dllist list){
	JRB output = make_jrb();
	JRB c, node, cn;

	c = jrb_find_gen(color, u, graph.func);
	c->val = new_jval_i(GREY);

	int n = outdegree(graph, u, output);

	jrb_traverse(node, output){
		cn = jrb_find_gen(color, node->key, graph.func);
		if(jval_i(cn->val) == WHITE)
			topo_Visit(graph, node->key, color, list);
	}

	c->val = new_jval_i(BLACK);
	dll_prepend(list,u);

	jrb_free_tree(output);
}

void topo(Graph graph, Jval s){
	if(!hasCycle(graph, 1)){
		JRB color = make_jrb();
		JRB node, c;

		jrb_traverse(node, graph.vertices){
			jrb_insert_gen(color, node->key, new_jval_i(WHITE), graph.func);
		}

		Dllist list = new_dllist();
		Dllist dnode;

		topo_Visit(graph, s, color, list);

		jrb_traverse(node, graph.vertices){
		 	c = jrb_find_gen(color, node->key, graph.func);
			if(jval_i(c->val) == WHITE)
				topo_Visit(graph, node->key, color, list);
		}

		dll_traverse(dnode, list){
			visitNode(jrb_find_gen(graph.vertices, dnode->val, graph.func));
		}

		free_dllist(list);
		jrb_free_tree(color);
	}
	else {
		printf("Oops! Graph has cycle\n");
	}
}

void printPath(Graph graph, JRB p, Jval f, Jval t){
	if(graph.func(f, t) == 0)
		printNodeVal(jrb_find_gen(graph.vertices, f, graph.func));
	else {
		JRB node;
		Jval prev;

		jrb_traverse(node, p){
			if(graph.func(node->key, t) == 0)
				prev = node->val;
			
		}
		printPath(graph, p, f , prev);
		printNodeVal(jrb_find_gen(graph.vertices, t, graph.func));
	}
}

int Dijkstra(Graph graph, Jval v1, Jval v2, int option){
	JRB p = make_jrb();
	JRB d = make_jrb();
	JRB node;

	pQueue Q = new_pQueue();

	jrb_traverse(node, graph.vertices){
		jrb_insert_gen(p, node->key, new_jval_v(NULL), graph.func);

		if(graph.func(node->key, v1) == 0){
			pQueue_insert(Q,0,node->key);
			jrb_insert_gen(d, node->key, new_jval_i(0), graph.func);
		}
		else{
			pQueue_insert(Q,INF,node->key);
			jrb_insert_gen(d, node->key, new_jval_i(INF), graph.func);
		}		
	}

	JRB tree, v, u, du, dv, prev;
	int dis, w;
	Jval vertex;

	while(!is_empty_pQueue(Q)) {

		u = pQueue_getMin(Q);

		dis = jval_i(u->key);
		vertex = u->val;
 
		jrb_delete_node(u);

		tree = (JRB)jval_v(jrb_find_gen(graph.edges, vertex, graph.func)->val);

		jrb_traverse(v,tree) {		
			dv = jrb_find_gen(d, v->key, graph.func);
			du = jrb_find_gen(d, vertex, graph.func);
			w = jval_i(v->val);
			if(jval_i(dv->val) > jval_i(du->val) + w){
				dv->val = new_jval_i(jval_i(du->val) + w);
				jrb_find_gen(p, v->key, graph.func)->val = vertex;

				decrease_key(Q, v->key, jval_i(dv->val), graph.func);
			}
		}
	}

	jrb_free_tree(Q);

	dv = jrb_find_gen(d, v2, graph.func);

	jrb_free_tree(d);

	if(jval_i(dv->val) >= INF) {
		// printf("There isn't any path from %d to %d\n", v1, v2);
		
		jrb_free_tree(p);
		return -1;
	}
	else {
		if(option == 1) {
			printPath(graph, p, v1, v2);
			printf("\n");
		}

		jrb_free_tree(p);

		return jval_i(dv->val);
	}
}

int SPFA(Graph graph, Jval v1, Jval v2, int option) {
	JRB p = make_jrb();
	JRB d = make_jrb();
	JRB inque = make_jrb();
	JRB count = make_jrb();
	JRB node;

	Queue Q = new_queue();

	int n = getGraphSize(graph);

	jrb_traverse(node, graph.vertices){
		jrb_insert_gen(p, node->key, new_jval_v(NULL), graph.func);

		if(graph.func(node->key, v1) == 0){
			enqueue(Q, v1);
			jrb_insert_gen(d, node->key, new_jval_i(0), graph.func);
		}
		else
			jrb_insert_gen(d, node->key, new_jval_i(INF), graph.func);

		jrb_insert_gen(inque, node->key, new_jval_i(0), graph.func);
		jrb_insert_gen(count, node->key, new_jval_i(0), graph.func);
	}

	JRB tree, v, du, dv, i, cnt;
	Jval u;
	int w;

	while(!is_empty_pQueue(Q)){
		u = dequeue(Q);

		i = jrb_find_gen(inque, u, graph.func);
		i->val = new_jval_i(0);

		tree = (JRB)jval_v(jrb_find_gen(graph.edges, u, graph.func)->val);

		jrb_traverse(v, tree){

			dv = jrb_find_gen(d, v->key, graph.func);
			du = jrb_find_gen(d, u, graph.func);
			w = jval_i(v->val);

			if(jval_i(dv->val) > jval_i(du->val) + w){
				dv->val = new_jval_i(jval_i(du->val) + w);
				jrb_find_gen(p, v->key, graph.func)->val = u;

				i = jrb_find_gen(inque, v->key, graph.func);

				if(!jval_i(i->val)) {
					cnt = jrb_find_gen(count, v->key, graph.func);
					cnt->val = new_jval_i(jval_i(cnt->val) + 1);
					if(jval_i(cnt->val) > n) {
						// printf("Graph has negative cycle\n");
						return -2;
					}

					enqueue(Q, v->key);
					i->val = new_jval_i(1);
				}
			}
		}
	}

	dv = jrb_find_gen(d, v2, graph.func);

	jrb_free_tree(d);
	jrb_free_tree(inque);
	jrb_free_tree(count);

	if(jval_i(dv->val) >= INF) {
		// printf("There isn't any path from %d to %d\n", v1, v2);
		jrb_free_tree(p);
		return -1;
	}
	else {
		if(option == 1) {
			printPath(graph, p, v1, v2);
			printf("\n");
		}
		jrb_free_tree(p);
		
		return jval_i(dv->val);
	}	
}

void dropGraph(Graph graph){
	JRB node;

	//free vertices tree
	jrb_traverse(node, graph.vertices){
		free(jval_s(node->val));
	}

	jrb_free_tree(graph.vertices);

	//free edges tree
	jrb_traverse(node, graph.edges){
		jrb_free_tree((JRB) jval_v(node->val));
	}
	jrb_free_tree(graph.edges);
}

