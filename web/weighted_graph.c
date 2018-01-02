#include "weighted_graph.h"

Graph createGraph()
{
	Graph graph;
	graph.edges = make_jrb();
	graph.vertices = make_jrb();
	return graph;
}

void addVertex(Graph graph, int id, char *name)
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return;
	JRB node = jrb_find_int(graph.vertices, id);
	if (node == NULL)
		jrb_insert_int(graph.vertices, id, new_jval_s(name));
}

void showVertexName(Graph graph, int id)
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return ;

	JRB node = jrb_find_int(graph.vertices, id);
	if (node != NULL) {
		printf("%s ", jval_s(node->val));
	}
}


char* getVertexName(Graph graph, int id){
  JRB node = jrb_find_int(graph.vertices,id);
  if(node == NULL) return NULL;
  else return jval_s(node->val);
}


int hasEdge(Graph graph, int v1, int v2)
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return 0;

	JRB node = jrb_find_int(graph.edges, v1);
	if (node == NULL)
		return 0;
	JRB tree = (JRB) jval_v(node ->val);
	JRB findV2 = jrb_find_int(tree, v2);
	if (findV2 != NULL)
		return 1;
	return 0;
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
	if (hasEdge(graph, v1, v2))
		return;

#define ADD_EDGE_MACRO(v1, v2) {  								\
		JRB node = jrb_find_int(graph.edges, v1); 					\
		JRB tree;												\
		if (node == NULL) 									\
		{														\
			tree = make_jrb();								\
			jrb_insert_int(graph.edges, v1, new_jval_v(tree)); 		\
		}														\
		else 													\
			tree = (JRB) jval_v(node->val); 				\
			jrb_insert_int(tree, v2, new_jval_d(weight)); 			\
	}	while(0);

	ADD_EDGE_MACRO(v1, v2);
#ifndef DIRECTED_GRAPH
	ADD_EDGE_MACRO(v2, v1);
#endif
#undef ADD_EDGE_MACRO

}


double getEdgeValue(Graph graph, int v1, int v2) {
  JRB node = jrb_find_int(graph.edges, v1);
  if(node == NULL) return INFINITY_VALUE;

  
  JRB tree = (JRB) jval_v(node->val);
  JRB nodeTree;
  jrb_traverse(nodeTree, tree) {
    if(jval_i(nodeTree->key) == v2) return jval_d(nodeTree->val);
  }
  return INFINITY_VALUE;
}

int outDegree(Graph graph, int v, int *output) {
	if(graph.edges == NULL || graph.vertices == NULL)
		return 0;
	JRB node = jrb_find_int(graph.edges, v);
	if(node == NULL)
		return;
	int total = 0;
	JRB tree = (JRB)jval_v(node->val);
	output[total++] = jval_i(node->key);
	return total;
}

int inDegree(Graph graph, int v, int *output) {
	if(graph.edges == NULL || graph.vertices == NULL)
		return 0;
	JRB node = jrb_find_int(graph.edges, v);
	if(node == NULL)
		return;
	int total = 0;
	jrb_traverse(node, graph.vertices){
		if(hasEdge(graph, jval_i(node->key), v))
			output[total++] = jval_i(node->key);
	}
	return total;
}

void BFS(Graph graph, int start, int stop, void (*visitFunc)(Graph, int))
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return;

	int max_id = getMaxId(graph);

	int *visited = (int*)malloc(sizeof(int) * (max_id + 1));
	if (visited == NULL) {
		fprintf(stderr, "Allocated failed in %s:%d \n", __FILE__, __LINE__);
		exit(1);
	}

	JRB node;
	jrb_traverse(node, graph.vertices)
	visited[jval_i(node->key)] = 0;

	Dllist queue = new_dllist();

	node = jrb_find_int(graph.vertices, start);
	if (node == NULL)
		goto end;


	dll_append(queue, new_jval_i(start));

	while (!dll_empty(queue))
	{
		Dllist node = dll_first(queue);
		int v = jval_i(node->val);
		dll_delete_node(node);

		if (visited[v] == 0)
		{
			visitFunc(graph, v);
			visited[v] = 1;
		}

		if (v == stop)
			goto end;

		JRB u = jrb_find_int(graph.edges, v);

		if (u == NULL)
			continue;

		JRB connect_to_u = (JRB)jval_v(u->val);
		JRB temp;
		jrb_traverse(temp, connect_to_u)
		{
			if (visited[jval_i(temp->key)] == 0)
				dll_append(queue, new_jval_i(temp->key.i));
		}
	}

end:
	printf("\n");

	free(visited);
	free_dllist(queue);
}

void DFS(Graph graph, int start, int stop, void (*visitFunc)(Graph, int))
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return;

	int max_id = getMaxId(graph);

	int *visited = (int*)malloc(sizeof(int) * (max_id + 1));
	if (visited == NULL) {
		fprintf(stderr, "Allocated failed in %s:%d \n", __FILE__, __LINE__);
		exit(1);
	}

	JRB node;
	jrb_traverse(node, graph.vertices)
	visited[jval_i(node->key)] = 0;

	Dllist stack = new_dllist();

	node = jrb_find_int(graph.edges, start);
	if (graph.edges == NULL)
		goto end;


	dll_append(stack, new_jval_i(start));
//prepare before visiting all vertices
	while (!dll_empty(stack))
	{
		Dllist node = dll_last(stack);
		int v = jval_i(node->val);
		dll_delete_node(node);

		if (visited[v] == 0)
		{
			visitFunc(graph, v);
			visited[v] = 1;
		}

		if (v == stop)
			goto end;

		JRB u = jrb_find_int(graph.edges, v);

		if (u == NULL)
			continue;

		JRB connect_to_u = (JRB)jval_v(u->val);
		JRB temp;
		jrb_traverse(temp, connect_to_u)
		{
			if (visited[jval_i(temp->key)] == 0)
				dll_append(stack, new_jval_i(temp->key.i));
		}
	}

end:
	printf("\n");

	free(visited);
	free_dllist(stack);
}


void DFS_all(Graph graph, void (*visitFunc)(Graph, int))
{
	int max_id = getMaxId(graph);

	int *visited = (int*)malloc(sizeof(int) * (max_id + 1));
	if (visited == NULL) {
		fprintf(stderr, "Allocated failed in %s:%d \n", __FILE__, __LINE__);
		exit(1);
	}
	int i;

	for (i = 0; i <= max_id; ++i)
		visited[i] = 0;

	for (i = 0 ; i <= max_id; ++i)
		if (!visited[i])
				DFS_ulti(graph, i, -1, visitFunc, visited);
	free(visited);
}

void DFS_ulti(Graph graph, int start, int stop, void (*visitFunc)(Graph, int), int *visited)
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return;

	Dllist stack = new_dllist();
	JRB node = jrb_find_int(graph.vertices, start);
	if (node == NULL)
		return;

	dll_append(stack, new_jval_i(start));
//prepare before visiting all vertices
	while (!dll_empty(stack))
	{
		Dllist node = dll_last(stack);
		int v = jval_i(node->val);
		dll_delete_node(node);

		if (visited[v] == 0)
		{
			visitFunc(graph, v);
			visited[v] = 1;
		}

		if (v == stop)
			return;

		JRB u = jrb_find_int(graph.edges, v);

		if (u == NULL)
			continue;

		JRB connect_to_u = (JRB) jval_v(u->val);
		JRB temp;
		jrb_traverse(temp, connect_to_u)
		{
			if (visited[jval_i(temp->key)] == 0)
				dll_append(stack, new_jval_i(temp->key.i));
		}
	}
	free_dllist(stack);
}

int isCyclicUtil(Graph graph, int vertex)
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return 0;

	int max_id = getMaxId(graph);

	int *visited = (int*)malloc(sizeof(int) * (max_id + 1));
	if (visited == NULL) {
		fprintf(stderr, "Allocated failed in %s:%d \n", __FILE__, __LINE__);
		exit(1);
	}

	JRB node;
	jrb_traverse(node, graph.vertices)
	visited[jval_i(node->key)] = 0;

	Dllist stack = new_dllist();

	node = jrb_find_int(graph.vertices, vertex);
	if (node == NULL)
		goto end;

	int flag = 0;

	dll_append(stack, new_jval_i(vertex));
//prepare before visit all  vertices

	while (!dll_empty(stack))
	{
		Dllist node = dll_last(stack);
		int v = jval_i(node->val);
		dll_delete_node(node);
		//pop an element from the stack

		if (visited[v] == 0)
		{
			visited[v] = 1;
			if (flag == 0 && v == vertex)
				visited[v] = 0;
		}

		if (v == vertex && visited[vertex] == 1)
		{
			free(visited);
			free_dllist(stack);
			return 1;
		}
		//there is a cycle

		flag = flag + 1; //start to run the inverse way from the beginning

		JRB v_node = jrb_find_int(graph.edges, v);

		if (v_node == NULL)
			continue;

		int *out_degree_list = (int *)malloc(sizeof(int) * (max_id + 1));
		if (out_degree_list == NULL) {
			fprintf(stderr, "%s %s:%d\n", "malloc failed in", __FILE__, __LINE__);
			exit(1);
		}
		//out degree vertices list of u

		int out_degree_num = outDegree(graph, v, out_degree_list);
		//number of out degree vertices of v
		if (out_degree_num == 0)
			continue;
		int i;
		for (i = 0; i < out_degree_num; ++i)
		{
			int temp = out_degree_list[i];
			if (visited[temp] == 0)
				dll_append(stack, new_jval_i(temp));
		}
		//traverse through the out degree list of vertex v
		free(out_degree_list);
	}

end:
	free(visited);
	free_dllist(stack);
	return 0;
}

int DAG(Graph graph)
{
	if (graph.edges == NULL || graph.vertices == NULL)
		return 0;
	JRB node;
	jrb_traverse(node, graph.vertices)
	{
		if (isCyclicUtil(graph, jval_i(node->key)))
			return 0;
	}
	return 1;
}

int getMaxId(Graph g) {
	if (g.edges == NULL || g.vertices == NULL)
		return 0;
	int max_id = 0;
	JRB tmp;
	jrb_traverse(tmp, g.vertices) {
		int key = jval_i(tmp->key);
		if (key > max_id)
			max_id = key;
	}
	return max_id;
}

int getMinId(Graph g) {
	if (g.edges == NULL || g.vertices == NULL)
		return 0;
	int min_id = 10000000;
	JRB tmp;
	jrb_traverse(tmp, g.vertices) {
		int key = jval_i(tmp->key);
		if (key < min_id)
			min_id = key;
	}
	return min_id;
}

void topologicalSort(Graph g, int *output, int *n, void (* visitFunc)(Graph, int))
{
    int i;
   	if (g.edges == NULL || g.vertices == NULL)
		return;
	Dllist queue = new_dllist(); //initialize an empty queue
	JRB temp;
	JRB node = jrb_find_int(g.vertices, getMinId(g));
	if (node == NULL)
		goto end;

	int max_id = getMaxId(g);
	int *in_degree_table = malloc(sizeof(int) * (max_id + 1)); //in degree table of the DAG
	if (in_degree_table == NULL) {
		fprintf(stderr, "%s in %s:%d !!\n", "malloc failed", __FILE__, __LINE__);
		exit(1);
	}

	int *in_degree_list_v = malloc(sizeof(int) * (max_id + 1)); //in degree list of v
	if (in_degree_list_v == NULL) {
		fprintf(stderr, "%s in %s:%d !!\n", "malloc failed", __FILE__, __LINE__);
		exit(1);
	}

	jrb_traverse(temp, g.vertices)
	{
		int v = jval_i(temp->key);
		if ((in_degree_table[v] = inDegree(g, v, in_degree_list_v)) == 0)
			dll_append(queue, new_jval_i(v));
	}
	free(in_degree_list_v);
	//enqueue all the vertices with in_degree == 0

	while (!dll_empty(queue))
	{
		Dllist node = dll_first(queue);
		int u = jval_i(node->val);
		dll_delete_node(node);
		//just the normal dequeue

		visitFunc(g, u);

		int *out_degree_list_u = malloc(sizeof(int) * (max_id + 1)); //out degree list of u
		//we use it to consider arc(u, w)
		if (out_degree_list_u == NULL) {
			fprintf(stderr, "%s in %s:%d !!\n", "malloc failed", __FILE__, __LINE__);
			exit(1);
		}

		int out_degree_num_u = outDegree(g, u, out_degree_list_u);
		//traverse through all out degree vertices of u
		if (out_degree_num_u != 0) // if we have out degree vertices of u
		{
			for (i = 0; i < out_degree_num_u; ++i)
			{
				int w = out_degree_list_u[i];
				in_degree_table[w] -= 1;

				if (in_degree_table[w] == 0)
					dll_append(queue, new_jval_i(w));
			}
		}
		free(out_degree_list_u);
	}
end:
	free(in_degree_table);
	free_dllist(queue);
}

void dropGraph(Graph graph)
{
	JRB node;
	jrb_traverse(node, graph.edges)
	{
		jrb_free_tree(jval_v(node->val));
	}
	jrb_free_tree(graph.vertices);
	jrb_free_tree(graph.edges);
	//maybe add graph == NULL then check if the context error is reduced or not
}

double getPath(JRB d, int u) {
  JRB node = jrb_find_int(d,u);
  if(node == NULL) return INFINITY_VALUE;

  return jval_d(node->val);
}

void relax(Graph graph, JRB d, JRB parent, int u, int v) {
  JRB node_d = jrb_find_int(d,v);
  JRB node_parent = jrb_find_int(parent, v);
  if( getPath(d,v) > getPath(d,u) +  getEdgeValue(graph,u,v)) {
    if(node_d != NULL) {
      double newWeight =  getPath(d,u) +  getEdgeValue(graph,u,v);
      node_d->val = new_jval_d( newWeight );
      node_parent->val = new_jval_i(u); 
    }
  }
}

int deQueue(Dllist queue, JRB d) {
  Dllist min;
  double minDistance = INFINITY_VALUE;
  Dllist node;
  
  dll_traverse(node, queue) {
    JRB nodeTree = jrb_find_int(d, jval_i(dll_val(node)) );
    if(jval_d(nodeTree->val) < minDistance ) {
      minDistance = jval_d(nodeTree->val);
      min = node;
    }
  }
  int shortestNode = jval_i(dll_val(min));;
  dll_delete_node(min);
  return shortestNode;
}

void Dijkstra(Graph graph, int s, JRB d, JRB parent) {
  JRB node;
  Dllist queue = new_dllist();
  jrb_traverse(node, graph.vertices) {
    jrb_insert_int(d, jval_i(node->key), new_jval_d(INFINITY_VALUE) );
    jrb_insert_int(parent, jval_i(node->key), new_jval_v());
    dll_prepend(queue, new_jval_i( jval_i(node->key)) );
  }
  

  node = jrb_find_int(d, s);
  if(node != NULL) node->val = new_jval_d(0);
  
  while(!dll_empty(queue)) {
    int u = deQueue(queue,d);
    JRB nodeTree = jrb_find_int(graph.edges, u);
    if(nodeTree != NULL) {
      JRB tree = (JRB) jval_v(nodeTree->val);
      jrb_traverse(node, tree) {
        relax(graph,d,parent,u,jval_i(node->key));
      }  
    }
  
  }
}



double recurrence(JRB d, JRB parent, int s) {
  JRB node = jrb_find_int(d, s);
  if(node == NULL) return 0;
  JRB nodeParent = jrb_find_int(parent,s);
  printf("%d\n", jval_i(nodeParent->val));
  return jval_d(node->val) + recurrence(d,parent, jval_i(nodeParent->val));
}

double shortestPath(Graph graph, int s, int t, int* path, int* length) {
  JRB d = make_jrb();
  JRB parent = make_jrb();

  Dijkstra(graph, s, d, parent);

  JRB node = jrb_find_int(d, t);
  if(node == NULL) return INFINITY_VALUE;
  double distance = jval_d(node->val);
  return distance;
}

JRB getAdjList(Graph g, int v)
{
	JRB found = jrb_find_int(g.edges, v);

	if(found){
		return (JRB) jval_v(found->val);
	}else
		return NULL;
}

int getAdjacentVertices(Graph g, int id, int* output)
{
	JRB tmp;
	int i = 0;
	JRB adjlist = getAdjList(g, id);
	if(!adjlist){
		return -1;
	}
	jrb_traverse(tmp, adjlist){
		output[i] = jval_i(tmp->key);
		i++;
	}

	return i;
}

void weight_increase(Graph g, int v1, int v2)
{

  JRB node1 = jrb_find_int(g.edges, v1);
  JRB node2 = jrb_find_int((JRB)jval_v(node1->val), v2);
  int w = jval_i(node2->val);
  node2->val = new_jval_i(w+ 1);

  node1 = jrb_find_int(g.edges, v2);
  node2 = jrb_find_int((JRB)jval_v(node1->val), v1);
  node2->val = new_jval_i(w + 1);
}
