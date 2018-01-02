#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bt/inc/btree.h"

#include "lib.h"

#define MAX 30


void correct(char *s) {
  int i;
  for (i = 0; i < strlen(s); i++)
    if (s[i] == '_')
      s[i] = ' ';
}

int readFile(FILE *f1, BTA *book) {
  char buff[100];
  int count = 0;
  char *name = (char*)malloc(30 * sizeof(char));	
  char* number = (char*)malloc(30 * sizeof(char));
	while(fgets(buff,100,f1) != NULL) {
		sscanf(buff, "%s %s", name, number);
		correct(name);
		int val = btins(book,name,number,MAX);
    if(val == 0) count++;
	}
	free(name);
  free(number);
  return count;
}

void find_key(BTA *book) {
  char name[MAX];
  char number[MAX];
  int rsize;

  getStr(name);

  int val = btsel(book, name, number, MAX, &rsize);
  if(val == 0) {
    printf("number: %s\n",number);  
  }
  else printf("Not found\n");
}


void add(BTA *book) {
  char number[MAX];

  Entry record;
  int rsize;
 
  record = getEntry();

  int val = btsel(book, record.name, number, MAX * sizeof(char), &rsize);

  if (val == 0) printf("Record existed\n");

  else {

    int check = btins(book, record.name, record.number, MAX * sizeof(char));
    if (check == 0) printf("Inserted\n");
    else printf("Error Occured\n");
  }
}

void edit(BTA *book) {
  char number[MAX];
  Entry record = getEntry();

  int rsize;

  int val = btsel(book, record.name, number, MAX * sizeof(char), &rsize);

  if(val != 0) printf("Record not exist\n");
  else {
    int check = btupd(book, record.name, record.number, MAX * sizeof(char));

    if (check == 0) printf("Record edited\n");

    else printf("Error \n");
  }
}

void delete(BTA *book) {
  char number[MAX];
  char name[MAX];

  int rsize;

  printf("Name: ");
  getStr(name);

  int val = btsel(book, name, number, MAX * sizeof(char), &rsize);

  if(val != 0) printf("Record not exist\n");
  else {
    int check = btdel(book, name);
    if(check == 0) printf("Record deleted\n");
    else printf("Error\n");
  }
}

void traverse (BTA *book) {
  char number[MAX];
  int rsize;
  char name[MAX];
  btsel(book,"",number, MAX * sizeof(char), &rsize);
  while( btseln(book,name,number, MAX * sizeof(char), &rsize) == 0 ) {
    printf("%-30s%s\n",name,number );
  }
}


int main()
{
  btinit();
  BTA *book = btopn("btphonebook.dat", 0, 0);

  int select = 1;
  int count;


  while (select != 0) {
    printf("1. Search\n");
    printf("2. Add\n");
    printf("3. Edit\n");
    printf("4. Delete\n");
    printf("5. Traverse\n");
    printf("0. Exit\n");

    printf("Select: ");
    scanf("%d%*c",&select);

    printf("\n\n");
    switch(select) {
      case 1: 
        find_key(book);
        break;
      case 2: 
        add(book);
        break;

      case 3: 
        edit(book);
        break;
      case 4: 
        delete(book);
        break;
      case 5: 
        traverse(book);
        break;
      case 0: break;
      default: 
        printf("Invalid\n");
        break;
    }

    printf("\n\n\n");
  }
  btcls(book);
  return 0;
}
