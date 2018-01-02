#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

typedef struct {
    char *name;
    char *number;
} Entry;



int getStr(char *word) {
  fgets(word, MAX, stdin);
  while(word[strlen(word) - 1] == '\n') word[strlen(word) - 1] = '\0';

  if(strlen(word) == 0 || word == NULL) return 0;
  else return 1;
}

Entry getEntry() {
	char name[MAX];
	char number[MAX];
	Entry record;
	record.name = (char*)malloc(MAX * sizeof(char));
	record.number = (char*)malloc(MAX * sizeof(char));
	printf("Name: ");
	getStr(name);
	strcpy(record.name, name);

	printf("Number: ");
	getStr(number);
	strcpy(record.number, number);

	return record;	
}
