#include "weighted_graph.h"

void trimLeft(char s[]){
  int i=0, j;
  while(s[0]==' '){
    for(j=0;j<strlen(s)-1;j++){
      s[j]=s[j+1];
    }
    s[strlen(s)-1]='\0';
  }
}

void trimRight(char s[]){
  int i=strlen(s)-1;
  while(s[i]==' '){
    s[i--]='\0';
  }
}

void trimMiddle(char s[]){
  int i=0,j;
  while(s[i]==' ') i++;
  for(i;i<strlen(s);i++){
    if((s[i]==' ')&&(s[i+1]==' ')){
  for(j=i;j<strlen(s)-1;j++){
    s[j]=s[j+1];
  }
  s[strlen(s)-1]='\0';
  i--;
      }
  }
}

// void toLowcase(char s[]){
//   for(int i=0;i<strlen(s);i++){
//     tolower(s[i]);
//   }
// }

void trim(char s[]){
  // toLowcase(s);
  trimLeft(s);
  trimRight(s);
  trimMiddle(s);
}

int split(char *s, char c, char **substr){
    int i = 0, n = 0, j;
    char *ss = malloc(strlen(s));

    while(i < strlen(s)){
        
        j = 0;

        for(i; i < strlen(s); i++){
            if(s[i] == c) break;
            ss[j++] = s[i];
        }
        ss[j] = '\0';
        trim(ss);
        
        substr[n++] = strdup(ss);

        i++;
    }

    free(ss);
    return n;
}

int getFileLine(FILE *fptr){
    char c;
    int n = 0;
    c = fgetc(fptr);
    while(c != EOF) {
        if(c=='\n') n++;
        c = fgetc(fptr);
    }
    fseek(fptr,0,SEEK_SET);
    return n;
}

int getNum(char *s){
    int num = 0;
    int i;
    for(i=0; i < strlen(s); i++){
        num = num*10 + s[i] - '0';
    }
    return num;
}

int importProduct(Graph graph, char *fileName) {
	FILE *f = fopen(fileName, "r");
	if(f == NULL) {
		printf("Can not open the file %s\n", fileName);
		return -1;
	}
	char name[100];
	int id, n=0;

	while(!feof(f)) {
		n++;
		fscanf(f, "%d %s\n", &id, name);
		printf("%d %s\n", id, name);
		addVertex(graph, id, name);
	}
	fclose(f);
	return n;
}

void importOrder(Graph g, char *fileName)
{
	FILE *f = fopen(fileName, "r");

	if(f == NULL)
	{
		printf("Can't open orderhistory.txt!!!\n");
		exit(1);
	}

	int p1, p2, p3;
	char c;

	printf("\n");
	while(!feof(f))
	{
		if(fscanf(f, "%d", &p1) > 0)
		{
			printf("%s ", getVertexName(g, p1));
			c = fgetc(f);
			if(c == '\n')
			{
				printf("\n");
				continue;
			}
			else if(c == EOF)
			{
				printf("\n");
				break;
			}

			fscanf(f, "%d", &p2);
			printf("%s ", getVertexName(g, p2));

			if(hasEdge(g, p1, p2) == 1)
			{
				weight_increase(g, p1, p2);
			}
			else
			{
				addEdge(g, p1, p2, 1);
			}

			c = fgetc(f);
			if(c == '\n')
			{
				printf("\n");
				continue;
			}
			else if(c == EOF)
			{
				printf("\n");
				break;
			}

			fscanf(f,"%d", &p3);
			printf("%s ", getVertexName(g, p3));

			if(hasEdge(g, p1, p3) == 1)
			{
				weight_increase(g, p1, p3);
			}
			else
			{
				addEdge(g, p1, p3, 1);
			}
			if(hasEdge(g, p2, p3) == 0)
			{
				addEdge(g, p2, p3, 1);
			}
			else
			{
				weight_increase(g, p2, p3);
			}

			c = fgetc(f);
			printf("\n");
		}
		else
			break;
	}
	
	printf("\n");
	fclose(f);
}

void related_list(Graph g)
{
	JRB list = make_jrb();
	JRB node, adj;
	int id;

	printf("Enter ID: ");
	scanf("%d", &id);

	adj = getAdjList(g, id);
	if(adj != NULL)
	{
		jrb_traverse(node, adj)
		{
			jrb_insert_int(list, jval_i(node->val), new_jval_i(jval_i(node->key)));

		}
	}
	else
	{
		printf("The product (ID: %d) has no relation\n", id);
		return;
	}

	printf("List of related product(s):");
	jrb_rtraverse(node, list)
	{
		// printf("%d\n", jval_i(node->key));
		printf(" %s", getVertexName(g, jval_i(node->val)));
		if(node != jrb_first(list))
		{
			printf(",");
		}
	}
	printf("\n");
}

void main(int argc, char **argv) {
	Graph graph = createGraph();
	JRB node, n;
	int id1, id2, w;
	JRB output, output_s;
	int choice;
	char sections[6][40] = {"List of products", "List of order", "level of relation between 2 products", "List of relevant products", "Show the relation between 2 products", "exit"};
	do{
		choice = getMenu(sections, 6);
		switch (choice) {
			case 1:
			importProduct(graph, "products.txt");
			jrb_traverse(node, graph.vertices) {
				printf("ID: %d\nName: %s\n", jval_i(node->key), getVertexName(graph, jval_i(node->key)));
			}
			break;
			case 2:
			printf("\n");
			importOrder(graph, "orderhistory.txt");
			break;
			case 3:
			printf("Input the id of the first product: \n");
			scanf("%d%*c", &id1);
			printf("input the id of the second product: \n");
			scanf("%d%*c", &id2);
			if(jrb_find_int(graph.vertices, id1) == NULL || jrb_find_int(graph.vertices, id2) == NULL)
				printf("Not exist\n");
			else {
				w = hasEdge(graph, id1, id2);
				printf("The level of relation between 2 products: %d\n", w);
			}
			break;
			case 4:
			related_list(graph);
            break;
			case 5:
			printf("enter the id of the first product: ");
			scanf("%d%*c", &id1);
			printf("Enter the id of the second product: ");
			scanf("%d%*c", &id2);
			if(jrb_find_int(graph.vertices, id1) == NULL || jrb_find_int(graph.vertices, id2) == NULL) {
				printf("not exist these id\n");
			} else {
				//if()
			}
			break;
			case 6:
			dropGraph(graph);
			printf("Done\n");
			break;
			default:
			printf("Wrong choice\n");
		}
	} while(choice != 6);
	return;
}