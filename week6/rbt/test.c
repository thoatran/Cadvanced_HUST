#include <stdio.h>
#include <stdlib.h>
#include "graph_rb.h"

int main()
{
  int i, n, output[100];
  Graph g = createGraph(100);
  addEdge(g,0,1);
  addEdge(g,0,2);
  addEdge(g,1,2);
  addEdge(g,1,3);
  n = getAdjacentVertices(g,1,output);
  if(n == 0) printf("No adjacent vetices of node 1\n");
  else {
  	printf("%d\n", n);
    printf("Adjacent vertices of node 1:\n");
    for (i = 0; i < n; ++i) {
      printf("%5d\n",output[i]);
    }

  }

  int ad = adjacent(g,0,1);
  if(ad == 1)
  	printf("0 and 1 is adjacent\n");
  else
  	printf("0 and 1 is not adjacent\n");
  dropGraph(g);
  return 0;
}
