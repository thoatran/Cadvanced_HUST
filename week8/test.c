#include "digraph.h"

int main()
{
    Graph g = createGraph();
    int output[10];
    int n;

    addVertex(g, 0, "0");
    addVertex(g, 1, "1");
    addVertex(g, 2, "2");
    addVertex(g, 3, "3");
    addVertex(g, 4, "4");
    addEdge(g, 1, 0);
    addEdge(g, 2, 1);
    addEdge(g, 3, 4);
    addEdge(g, 4, 0);



    printf("BFS order: \n");
    BFS(g, 2, 0, showVertexName);
    printf("\n");

    printf("DFS order: \n");
    DFS(g, 2, 0, showVertexName);
    printf("\n");

    printf("DFS_all order: \n");
    DFS_all(g, showVertexName);
    printf("\n");


    if (DAG(g) == 0)
        printf("Can not make topological sort\n");
    else
    {
        printf("The topological order: \n");
        topologicalSort(g, output, &n, showVertexName);
        printf("\n");
    }
    dropGraph(g);
    // free(list);
    return 0;
}
