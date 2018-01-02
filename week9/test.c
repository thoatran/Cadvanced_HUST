#include <stdio.h>
#include <stdlib.h>
#include "weighted_graph.h"

void printVertex(int v) {
  printf("%4d",v);
}


int main()
{
  int s,t,length, path[100];
  
  Graph g = createGraph();

  addVertex(g,1,"V1");
  addVertex(g,2,"V2");
  addVertex(g,3,"V3");
  addVertex(g,4,"V4");
  addVertex(g,5,"V5");
  addVertex(g,6,"V6");
  addVertex(g,7,"V7");
  addVertex(g,8,"V8");
  
  addEdge(g,1,2,9);
  addEdge(g,1,6,14);
  addEdge(g,1,7,15);
  addEdge(g,2,3,24);
  addEdge(g,3,8,19);
  addEdge(g,3,5,2);
  addEdge(g,6,3,18);
  addEdge(g,6,5,30);
  addEdge(g,6,7,5);
  addEdge(g,7,5,20);
  addEdge(g,7,8,44);
  addEdge(g,5,4,11);
  addEdge(g,5,8,16);
  addEdge(g,4,3,6);
  addEdge(g,4,8,6);
  
  s = 1;
  t = getMaxId(g);
  double weight = shortestPath(g,s,t,path,&length);
  if(weight == INFINITY_VALUE) {
    printf("No path between %d and %d\n",s,t);
  } else {
    printf("Weight = %.0lf\n", weight);
  }
  
return 0;
}

