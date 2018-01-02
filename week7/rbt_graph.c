#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/jrb.h"
#include "lib/dllist.h"


#define MAX 10
typedef JRB Graph;

Graph createGraph(){
  Graph g = make_jrb();
  return g;
}

void addEdgeList(Graph graph, char* v1, char* v2){
  Graph node = jrb_find_str(graph,v1);
  if(node == NULL) {
    Graph tree = make_jrb();
    jrb_insert_str(graph,v1, new_jval_v(tree));
    jrb_insert_str(tree,v2,new_jval_i(1));
  } else {
    Graph tree = (JRB) jval_v(node->val);
    jrb_insert_str(tree, v2, new_jval_i(1));
  } 
}

void addEdge(Graph graph, char* v1, char* v2){
  addEdgeList(graph,v1,v2);
  addEdgeList(graph,v2,v1);
}

int adjacent(Graph graph, char* v1, char* v2){
  Graph node = jrb_find_str(graph, v1);
  if(node == NULL) return 0;
  else {
    
    Graph tree = jrb_find_str((JRB)jval_v(node->val), v2);
    if(tree == NULL) return 0;
    else return 1;
  }
}

int getAdjacentVertices(Graph graph, char* v, char *output) {
  Graph node = jrb_find_str(graph, v);
  Graph tree = (JRB) jval_v(node->val);
  int total = 0;
  jrb_traverse(node, tree) {
    
    strcpy(output + total*MAX, jval_s(node->key));
    total++;
  }
  return total;
}

void dropGraph(Graph graph) {
  JRB node;
  jrb_traverse(node, graph) {
    jrb_free_tree(jval_v(node->val));
  }
}

void BFS(Graph graph, char* start, char* stop, void (*func)(char*)) {
  Graph node = jrb_find_str(graph, start);
  JRB visited = make_jrb();
  Dllist queue = new_dllist();
  dll_append(queue, new_jval_s(jval_s(node->key)));
  Dllist node_list;
 
  
  while(!dll_empty(queue)) {
    node_list = dll_last(queue);
    char* u =  strdup( jval_s(dll_val(node_list)) );
    dll_delete_node(node_list);
    if(jrb_find_str(visited, u) == NULL) {
      (*func)(u);
    
      jrb_insert_str(visited, u, new_jval_i(1));
      if(strcmp(u, stop) == 0) break;
      
      Graph node_graph = jrb_find_str(graph,u);
      Graph tree = (Graph) jval_v(node_graph->val);
      jrb_traverse(node, tree) {
  if(jrb_find_str(visited, jval_s(node->key)) == NULL) dll_append(queue, new_jval_s(jval_s(node->key)));
      }
    }
  }
}


void DFS(Graph graph, char* start, char* stop, void (*func)(char*)) {
  Graph node = jrb_find_str(graph, start);
  JRB visited = make_jrb();
  Dllist queue = new_dllist();
  dll_append(queue, new_jval_s(jval_s(node->key)));
  Dllist node_list;
  
  while(!dll_empty(queue)) {
    node_list = dll_first(queue);
    char* u =  strdup( jval_s(dll_val(node_list)) );
    dll_delete_node(node_list);
    if(jrb_find_str(visited, u) == NULL) {
      (*func)(u);
   
      jrb_insert_str(visited, u, new_jval_i(1));
      if(strcmp(u, stop) == 0) break;
      
      Graph node_graph = jrb_find_str(graph,u);
      Graph tree = (Graph) jval_v(node_graph->val);
      jrb_traverse(node, tree) {
  if(jrb_find_str(visited, jval_s(node->key)) == NULL) dll_append(queue, new_jval_s(jval_s(node->key)));
      }
    }
  }
}

int findPath(Graph graph, char* start, char* stop, Graph path) {
  Graph node = jrb_find_str(graph, start);
  JRB visited = make_jrb();
  Dllist queue = new_dllist();
  dll_append(queue, new_jval_s(jval_s(node->key)));
  Dllist node_list;
  
  jrb_insert_str(path, start, new_jval_s("-1"));
  
  while(!dll_empty(queue)) {
    node_list = dll_last(queue);
    char* u =  strdup( jval_s(dll_val(node_list)) );
    dll_delete_node(node_list);

    if(jrb_find_str(visited, u) == NULL) {
    
      jrb_insert_str(visited, u, new_jval_i(1));
      if(strcmp(u, stop) == 0) {
        return 1;
      }
      Graph node_graph = jrb_find_str(graph,u);
      Graph tree = (Graph) jval_v(node_graph->val);
      jrb_traverse(node, tree) {
        if(jrb_find_str(visited, jval_s(node->key)) == NULL) {
          dll_append(queue, new_jval_s(jval_s(node->key)));
          jrb_insert_str(path, jval_s(node->key), new_jval_s(u));
        }
      }
    }
  }
  return 0;
}

void readPath(Graph path, char* start, char* stop, char* strPath, int* numStations) {
  if( strcmp(stop,"-1") == 0 ) return;

  JRB node = jrb_find_str(path, stop);
  if(node == NULL) return;
  else {
    strcpy(strPath + *numStations * 10, stop);
    (*numStations)++;
    readPath(path, start, jval_s(node->val), strPath, numStations);  
  }
  
}

int findShortestPath(Graph graph, char* start, char* stop) {
  Graph path = make_jrb();
  char* strPath = (char*)malloc(100 * sizeof(char));
  int numStations = 0;
  int pathExist = findPath(graph, start, stop, path);
  if(pathExist) {
    readPath(path, start, stop, strPath, &numStations);
    for (int i = numStations - 1; i >= 0; i--)
    {
      printf("%-5s", strPath + i * 10);
    }
    printf("\n");
    return 1;
  }
  printf("Path not exist\n");
  return 0;
}