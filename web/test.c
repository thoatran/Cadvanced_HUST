#include "weighted_graph.h"

void size(char *fileName) {
	int n, m, count = 0;
	char tmp[99];
	FILE *f = fopen(fileName, "r");
	if(f == NULL) {
		printf("can not open the file %s\n", fileName);
		return;
	}
	fscanf(f, "%d\n", &n);
	while(count != n) {
		fscanf(f, "%[^\n]\n", tmp);
		count++;
	}
	fscanf(f, "%d\n", &m);
	printf("Number of URLs: %d\nNumber of superlinks: %d\n", n, m);
	fclose(f);
	return;
}

Graph readFile(char *fileName) {
	FILE *f = fopen(fileName, "r");
	if(f == NULL){
		printf("can not open the file %s\n", fileName);
		exit(1);
	}
	Graph graph = createGraph();
	int n, m , count = 0, v1, v2;
	char url[100];
	fscanf(f, "%d", &n);
	while(count != n) {
		fscanf(f, "%s %d\n", url, &v1);
		printf("%s %d\n", url, v1);
		addVertex(graph, v1, url);
		count ++;
	}
	fscanf(f, "%d\n", &m);
	count = 0;
	while(count !=m) {
		if(fscanf(f, "%d %d\n", &v1, &v2) != EOF){
			addEdge(graph, v1, v2, 1);
			count ++;
		}
		else
			goto error;
	}
	fclose(f);
	return graph;
	error:
	printf("File %s dose not follow the formal\n", fileName);
	exit(1);
}

void out(char *fileName, int id) {
	Graph graph = readFile(fileName);
	JRB node, adj;
	adj = getAdjList(graph, id);
	if(adj) {
		jrb_traverse(node, adj) {
			printf("%d %s\n", jval_i(node->key), getVertexName(graph, jval_i(node->key)));
		}
	} else {
		printf("The website (id: %d) does not have any link.\n", id);
	}
	dropGraph(graph);
}

int inde(Graph g, int v)
{
	JRB node, node1, adj;
	int total=0;
		
	jrb_traverse(node, g.edges)
	{
		adj = (JRB)jval_v(node->val);
		jrb_traverse(node1, adj)
		{
			if(jval_i(node1->key) == v)
			{
				total++;
			}
		}
	}
	return total;
	
}

void maxin(char *filename)
{
	Graph g = readFile(filename);
	JRB node;
	int MAX=0;
	int v, in_degree;
	
	jrb_traverse(node, g.vertices)
	{
		in_degree = inde(g, jval_i(node->key));
		if(MAX < in_degree)
		{
			MAX = in_degree;
			v = jval_i(node->key);
		}
	}

	printf("The website has the most in-links: %s (id: %d)\n", getVertexName(g, v), v);
	dropGraph(g);
}

void selfref(char *fileName) {
	Graph graph = readFile(fileName);
	JRB node, node1, adj;
	jrb_traverse(node, graph.vertices) {
		adj = getAdjList(graph, jval_i(node->key));
		if(adj) {
			jrb_traverse(node1, adj) {
				if(jval_i(node1->key) == jval_i(node->key)){
					printf("%d\n", jval_i(node->key));
					printf("The website having the link to itself is: %s\n", getVertexName(graph, jval_i(node->key)));
					printf("\n");
					return;
				}
			}
		}
	}
	printf("OK\n");
	dropGraph(graph);
}

void BFS_level_k(char *fileName, int id, int k) {
	if(k < 1)
	{
		printf("The parameter K must be positive!!!\n");
		exit(1);
	}
	Graph graph = readFile(fileName);
	JRB node;
	JRB vertex = jrb_find_int(graph.vertices, id);
	int *output;
	if(vertex == NULL){
		printf("does not exist this vertice on the graph\n");
		return;
	}
	printf("BFS order:\n");
	Dllist q;
	JRB v;
	int ver, n, i, count = 0;
	int adj[100];
	jrb_traverse(v, graph.vertices) {

	}
}

int main(int argc, char **argv) {
		if(argc > 5)
	{
		printf("Wrong syntax!!!\n");
		return 1;
	}

	if(strcmp(argv[1], "about") == 0)
	{
		printf("The exam for advanced programming C HK2 - 2014 - 2015.\n");
		return 0;
	}

	if(strcmp(argv[1], "size") == 0)
	{
		if(argc != 3)
		{
			printf("Wrong syntax!!!\n");
			return 1;
		}
		size(argv[2]);
		return 0;
	}

	if(strcmp(argv[1],"out") == 0)
	{

		if(argc != 4)
		{
			printf("Wrong syntax!!!\n");
			return 1;
		}

		out(argv[2], atoi(argv[3]));
		return 0;
	}

	if(strcmp(argv[1], "maxin") == 0)
	{
		if(argc != 3)
		{
			printf("Wrong syntax!!!\n");
			return 1;
		}
		maxin(argv[2]);
		return 0;
	}

	if(strcmp(argv[1], "selfref") == 0)
	{
		if(argc != 3)
		{
			printf("Wrong syntax!!!\n");
			return 1;
		}
		selfref(argv[2]);
		return 0;
	}

	if(strcmp(argv[1],"list") == 0)
	{

		if(argc != 5)
		{
			printf("Wrong syntax!!!\n");
			return 1;
		}

		BFS_level_k(argv[2], atoi(argv[3]), atoi(argv[4]));
		return 0;
	}

	
	printf("Wrong syntax!!!\n");
	return 0;
}