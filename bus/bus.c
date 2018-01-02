#include <libfdr/jrb.h>
#include <libfdr/dllist.h>
//../../lib/include/libfdr/jrb.h
#include <utility.h>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HuffmanTree.h"

void MENU(){
	printf("\n           MENU\n");
	printf("1. DISPLAY ALL STATIONS\n");
	printf("2. FIND PATH BETWEEN 2 STATION\n");
	printf("3. EXIT\n");
}

void printStation(Graph bus_graph, int n){
	printf(" ___________________________________________________________________________________________\n");
	printf("|___STT___|__________________STATION___________________________________|_________ID_________|\n");
	printf("|         |                                                            |                    |\n");
	for(int i = 0; i < n; i ++)
		printf("|%-.3d      | %-59s|%-20d|\n", i + 1, getVertexName(bus_graph, i), i);
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

int isStationFound(JRB ID_bus, char* location){
	//check if there is any station nearby location
	JRB tmp = NULL;
	jrb_traverse(tmp, ID_bus){
		if(strstr(tmp->key.s, location) != NULL)
			return 1;
	}
	return 0;
}

int find_ID(JRB ID_bus, int* id_station){
	//return 1 if successfully found
	//return 0 if unsucessfully found
	//return -1 to exit

	int i = 0;
	char location[256];
	JRB tmp = NULL;

	//read location from user
	printf("Location (enter \"EXIT\" to exit): ");
	scanf("%[^\n]s", location);
	getchar();	

	//check EXIT condition
	if(strcmp(location, "EXIT") == 0) return -1;

	//check if the location exists
	if(isStationFound(ID_bus, location) == 0){
		printf("Cannot find station at location: %s\n", location);
		printf("Please Re-try!\n\n");
		*id_station = -1;
		return 0;
	}


	//print list of stations nearby the location
	printf(" ___________________________________________________________________________________________\n");
	printf("|___STT___|__________________STATION___________________________________|_________ID_________|\n");
	printf("|         |                                                            |                    |\n");
	jrb_traverse(tmp, ID_bus){
		if(strstr(tmp->key.s, location) != NULL){
			printf("|%-.3d      | %-59s|%-20d|\n", ++i, tmp->key.s, tmp->val.i);
		}
	}
	printf("|_________|____________________________________________________________|____________________|\n");
	
		
	//get station_ID from user
	printf("\nChoose station ID: ");
	scanf("%d", id_station);
	getchar();

	//print information of station if the station_ID is valid
	jrb_traverse(tmp, ID_bus){
		if(tmp->val.i == *id_station){
			printf("STATION: %s\nID: %d\n\n", tmp->key.s, *id_station);
			return 1;
		}
	}

	//Notice when the station_ID is invalid
	printf("Cannot find the station with ID: %d\n", *id_station);
	printf("Please Re-try!\n\n");
	*id_station = -1;
	return 0;
}

void findPath(Graph bus_graph, JRB ID_bus){
	int flag;	
	double distance;
	int id_station1 = -1;
	int id_station2 = -1;


	while(id_station1 == -1){
		printf("DEPARTURING:\n");
		flag = find_ID(ID_bus, &id_station1);
		if(flag == -1) return;
		else if (flag == 1) break;
		else id_station1 = -1; 
	}


	while(id_station2 == -1){
		printf("DESTINATION:\n");
		flag = find_ID(ID_bus, &id_station2);
		if(flag == -1) return;
		else if (flag == 1) break;
		else id_station2 = -1; 
	}

	int length = 0;	//length of the path
 	int path[100];	
 	distance = shortestPath(bus_graph, id_station1, id_station2, path, &length);

 	if(distance == INT_MAX){
 		printf("NO PATH\n");
 		return;
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
			
			case 3:
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