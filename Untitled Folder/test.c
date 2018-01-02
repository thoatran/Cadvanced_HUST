#include "weighted_graph.h"

#define MAXSECTION 


void main(int argc, char **argv)
{
	int choice;
	char sections[MAXSECTION][40] = {};
	do {
		choice = getMenu(sections, MAXSECTION);
		switch (choice)
		{
		case 1:
		default:
			printf("Choice must be from 1 to %d\n", MAXSECTION);
			exit(1);
		}
	} while (choice != MAXSECTION);
}
