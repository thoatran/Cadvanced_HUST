#include <libfdr/jrb.h>
#include <libfdr/dllist.h>
//../../lib/include/libfdr/jrb.h
#include <utility.ｋｂh>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HuffmanTree.h"



void MENU(){
	printf("     MENU\n");
	printf("1. DISPLAY ALL STATIONS\n");
	printf("2. FIND PATH BETWEEN 2 STATION\n");
	printf("3. EXIT\n");
}

void printStation(Graph bus_graph, int n){
	printf(" ___________________________________________________________________________________________\n");
	printf("|___STT___|__________________STATION___________________________________|_________ID_________|\n");
	printf("|         |                                                            |                    |\n");
	for(int i = 0; i < n; i ++)
		printf("|%-0.3d      | %-59s|%-20d|\n", i + 1, getVertexName(bus_graph, i), i);
	printf("|_________|____________________________________________________________|____________________|\n");
	
}

void readStation(Graph bus_graph, JRB ID_bus, int *n, FILE* f1){
	char buffer[256];
	fscanf(f1, "%d%*c", n);

	for(int i = 0; i < *n; i ++){
		fgets(buffer, 256, f1);
		if(buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';
		addVertex(bus_graph, i, buffer);
		jrb_insert_str(ID_bus, strdup(buffer), new_jval_i(i));
	}
}

void readDistance(Graph bus_graph, JRB ID_bus, FILE* f2){
	char buffer[256];
	char station1[256];
	char station2[256];
	double distance;
	int id_station1;
	int id_station2;

	while((fgets(station1, 256, f2)) != NULL){

		if(station1[strlen(station1)-1] == '\n')
			station1[strlen(station1) - 1] = '\0';
		
		fgets(station2, 256, f2);
		if(station2[strlen(station2)-1] == '\n')
		station2[strlen(station2) - 1] = '\0';

		fgets(buffer, 256, f2);
		if(buffer[strlen(buffer)-1] == '\n')
		buffer[strlen(buffer)-1] = '\0';
		distance = (double) atof(buffer);

		if(jrb_find_str(ID_bus, station1) == NULL)
			printf("CANNOT find \"%s\"\n", station1);

		if(jrb_find_str(ID_bus, station2) == NULL)
			printf("CANNOT find \"%s\"\n", station2);

		id_station1 = jrb_find_str(ID_bus, station1)->val.i;
	 	id_station2 = jrb_find_str(ID_bus, station2)->val.i;
		addEdge(bus_graph, id_station1, id_station2 , distance);
	}
}

void findPath(Graph bus_graph, JRB ID_bus){
	char station1[256];
	char station2[256];
	double distance;
	int id_station1;
	int id_station2;

	printf("Departuring station: ");
	scanf("%[^\n]s", station1);
	getchar();	
	printf("Destination station: ");
	scanf("%[^\n]s", station2);
	getchar();
	
	if(jrb_find_str(ID_bus, station1) == NULL){
		printf("Cannot find station: %s\n", station1);
		return;
	}

	if(jrb_find_str(ID_bus, station2) == NULL){
		printf("Cannot find station: %s\n", station2);
		return;
	}

	id_station1 = jrb_find_str(ID_bus, station1)->val.i;
	id_station2 = jrb_find_str(ID_bus, station2)->val.i;

	printf("%s __TO__ %s:\n", getVertexName(bus_graph,id_station1), getVertexName(bus_graph,id_station2));

	int length = 0;
 	int path[100];
 	distance = shortestPath(bus_graph, id_station1, id_station2, path, &length);
 	if(distance == INT_MAX){
 		printf("NO PATH\n");
 		return 0;
 	}
 	printf("Shortest distance: %.2f\n", distance);
 	printf("Path:\n");
 	for(int i = length - 1; i >= 0 ; i --)
 		printf("Station %d: \t  %s\n", length - i, getVertexName(bus_graph, path[i]));

 	printf("\n");

}


int main(int argc, char const *argv[])
{
	FILE* f1;
	if((f1 = fopen("STATION_DATA", "r")) == NULL){
		printf("Cannot read Station file.\n");
		return 0;
	}

	FILE* f2;
	if((f2 = fopen("distance", "r")) == NULL){
		printf("Cannot read distance file.\n");
		return 0;
	}

	Graph bus_graph = createGraph();
	JRB ID_bus = make_jrb();	//key: name, val: id

	int n;
	int choice;

	readStation(bus_graph, ID_bus, &n, f1);
	readDistance(bus_graph, ID_bus, f2);

	do{
		MENU();
		printf("\nChoice: ");
		scanf("%d", &choice);
		getchar();

		switch (choice){
			case 1:
			printStation(bus_graph , n);
				break;
			
			case 2:
			findPath(bus_graph, ID_bus);
				break;
			
			default:
				printf("Invalid value for choice: %d\n\n", choice);
				break;
		}

	}while(choice != 3);

	fclose(f1);
	fclose(f2);

	return 0;
}