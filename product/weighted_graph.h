#include <libfdr/jrb.h>
#include <libfdr/dllist.h>
//../../lib/include/libfdr/jrb.h
#include <utility.h>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INFINITY_VALUE 10000000000
typedef struct {
	JRB edges;
	JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertexName(Graph graph, int id);
int hasEdge(Graph graph, int v1, int v2);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int inDegree(Graph graph, int v, int *output);    //traverse: start from all vertices to 1 vertex;
int outDegree(Graph graph, int v, int *output);    //traverse:start from a vertex v to all outer vertices;
void showVertexName(Graph graph, int id);
int getMaxId(Graph g);
int getMinId(Graph g);
void BFS(Graph graph, int start, int stop, void (*visitFunc)(Graph, int));
void DFS(Graph graph, int start, int stop, void (*visitFunc)(Graph, int));
void DFS_all(Graph graph, void (*visitFunc)(Graph, int));
//apply wwhen graph is not connected
void DFS_ulti(Graph graph, int start, int stop, void (*visitFunc)(Graph, int), int *visited);
int isCyclicUtil(Graph graph, int vertex);
//check for vertex v about existing a cycle
int DAG(Graph graph);
//check for acylic graph
void topologicalSort(Graph g, int *output, int *n, void (*visitFunc)(Graph, int));
void dropGraph(Graph graph);
double getPath(JRB d, int u);
void relax(Graph graph, JRB d, JRB parent, int u, int v);
int deQueue(Dllist queue, JRB d);
void Dijkstra(Graph graph, int s, JRB d, JRB parent);
double recurrence(JRB d, JRB parent, int s);
double shortestPath(Graph graph, int s, int t, int* path, int* length);
JRB getAdjList(Graph g, int v);
int getAdjacentVertices(Graph g, int id, int* output);
void weight_increase(Graph g, int v1, int v2);
void changeEdgeWeight(Graph graph, int v1, int v2, double newWeight);
void deleteEdge(Graph graph, int v1, int v2);	//return 1 if succeed, 0 otherwise
void deleteVertex(Graph graph, int id);