#include "weighted_graph.h"

#define MAXSECTION 5

typedef struct{
	char name[100];
	int friends_num;
} person;

typedef struct {
	Graph graph;
	person *list;
	int number;
	int relationship_num;
} map;

map input;

int compareName(char p1[], map data);
map inputFromFile(char fileName[]);
void showVertices(map data);
void showNode(int vertex);
void showAdjacent(char name[]);
void printOut(map data);
void hasCommonFriends(char name[], map data);

void main(int argc, char **argv) {
	int choice;
	char search[100];
	char sections[MAXSECTION][40] = {"Input from file", "Search for friends", "Print out", "Show people have common friends", "Exit"};
	do {
		choice = getMenu(sections, MAXSECTION);
		switch(choice) {
			case 1:
			input = inputFromFile(argv[1]);
			showVertices(input);
			printf("\n");
			break;
			case 2:
			printf("Type in a name: \n");
			scanf("%[^\n]", search);
			myfflush();
			showAdjacent(search);
			break;
			case 3:
			printf("Report: \n");
			printOut(input);
			break;
			case 4:
			printf("Type in a name: \n");
			scanf("%[^\n]", search);
			myfflush();
			hasCommonFriends(search, input);
			break;
			case MAXSECTION:
			free(input.list);
			dropGraph(input.graph);
			break;
			default:
			printf("Your choice must be from 1 to 5\n");
			exit(1);
		}
	} while(choice != MAXSECTION && choice != 0);
}

int compareName(char p1[], map data) {
	int i = 0;
	for( i = 0; i < data.number; i++) {
		if(strcmp(p1, data.list[i].name) == 0)
			return i;
	}
	return -1;

}

map inputFromFile(char fileName[]) {
	int i; 
	int j = 0;
	map data;
	int v1, v2;
	char p1[100], p2[100];
	FILE *f = fopen(fileName, "r");
	if(f == NULL){
		fprintf(stderr, "can't open file %s \n", fileName);
		exit(1);
	}

	fscanf(f, "%d\n", &data.relationship_num);
	data.list = (person*)malloc(sizeof(person) * data.relationship_num * 2);
	data.graph = createGraph();
	data.number = 0;

	for( i = 0; i < data.relationship_num; i++) {
		fscanf(f, "\"%[^\"]\" \"%[^\"]\"\n", p1, p2);
		if(compareName(p1, data) == -1)
			strcpy(data.list[data.number++].name , p1);
		v1 = compareName(p1, data);
		if(compareName(p2, data) == -1)
			strcpy(data.list[data.number++].name , p2);
		v2 = compareName(p2, data);
		addEdge(data.graph, v1, v2, 0);
	}

	for( i = 0; i < data.number; i++) {
		data.list[i].friends_num = 0;
	}
	printf("Input success\n");
	fclose(f);
	return data;
}

void showVertices(map data)
{
	int i, j;
	printf("Total vertices: %d\n", data.number);
	printf("Total edges: %d\n", data.relationship_num);
}

void showNode(int vertex)
{
	printf(" %s ", input.list[vertex].name);
}

void showAdjacent(char name[])
{
	int i;
	int v1 = -1;
	int output[10];
	int numberofAdjacent;

	for (i = 0; i < input.number; ++i)
		if (strcmp(input.list[i].name, name) == 0)
		{
			v1 = i;
			break;
		}
	if (v1 != -1)
	{
		numberofAdjacent = getAdjacentVertices(input.graph, v1, output);
		// printf("%d\n", numberofAdjacent);
		printf("Friends of %s are: ", name);
		for (i = 0; i < numberofAdjacent; ++i)
			showNode(output[i]);
		printf("\n\n");
	}
	else
		printf("Wrong name\n");
}


void printOut(map data)
{
	int max = 0;
	int i, j;
	int temp[data.number];
	int numberfriends[data.number];
	for (i = 0; i < data.number; ++i)
	{
		data.list[i].friends_num = getAdjacentVertices(data.graph, i, temp);
		numberfriends[i] = data.list[i].friends_num;
		if (max < numberfriends[i])
			max = numberfriends[i];
	}


	for (j = 1; j <= max; ++j)
	{
		printf("%d ", j);
		for (i = 0; i < data.number; ++i)
			if (data.list[i].friends_num == j)
				printf(" \"%s\" ", data.list[i].name);
		printf("\n");
	}
}

void hasCommonFriends(char name[], map data)
{
	int numberofAdjacent;
	int v1;
	int v2;
	int i;
	int j;
	int flag = 0;
	int visited[data.number];
	int output[data.number];

	for (i = 0; i < data.number; ++i)
		visited[i] = 0;
	for (i = 0; i < data.number; ++i)
		if (strcmp(data.list[i].name, name) == 0)
		{
			v1 = i;
			break;
		}
	if (v1 != -1)
	{
		numberofAdjacent = getAdjacentVertices(data.graph, v1, output);
		// printf("%d\n", numberofAdjacent);
		printf("People have common friends with %s are: ", name);
		for (i = 0; i < numberofAdjacent; ++i)
		{
			v2 = output[i];
			for (j = 0; j < data.number; ++j)
			{
				if (hasEdge(data.graph, v2, j))
				{
					if (j != v1 && visited[j] != 1)
					{
						showNode(j);
						visited[j] = 1;
					}
					flag = 1;
				}
			}
		}
		if (flag == 0)
			printf("No common friends\n");
		printf("\n\n");
	}
	else
		printf("Wrong name\n");
}