#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inc/btree.h"


void readFile(BTA *root) {
    FILE *f = fopen("phonebook.txt", "r");
    if(f == NULL) return;

    char name[30], number[20];

    while(!feof(f)) {
        fscanf(f,"%[^:]", name); fscanf(f,"%*c");
        fscanf(f,"%[^\n]", number); fscanf(f,"%*c");
        btins(root, number, name, 30* sizeof(char));
        // printf("%s-%s\n",name, number);
    }

    fclose(f);
}

void writeFile(char *name, char* number) {
    FILE *f = fopen("phonebook.txt","a");

    if(f == 0) {
        printf("cannot read file!\n");
        return;
    };

    fprintf(f,"\n%s:%s",name,number);

    fclose(f);
}

void insert(BTA *root) {
    char name[30], number[20];

    printf("input name: "); scanf("%[^\n]",name); scanf("%*c");
    printf("input number: "); scanf("%[^\n]",number); scanf("%*c");

    int a = btins(root, number, name, 30* sizeof(char));

    if(a) printf("insert fail!\n");

    else {
        writeFile(name, number);
        printf("insert ok!\n");
    }
}

void searchByName(BTA *root) {
    char name[30],name1[30];
    char number[15];
    int i, rsize, flag = 0;
    printf("enter the name: ");
    scanf("%[^\n]",name1); scanf("%*c");
    
    btpos(root,ZSTART);
    
    while(bnxtky(root,number,&i) == 0) {
        btsel(root, number, name, 30, &rsize);
        if(!strcmp(name,name1)) {
            printf("%-30s%-15s\n",name,number);
            flag = 1;
        }
    }

    if(!flag) printf("no number match!\n");
}

void searchByNumber(BTA *root) {
    char name[30], number[15];
    int rsize;
    
    printf("enter the number: ");
    scanf("%[^\n]",number); scanf("%*c");

    if(btsel(root, number,name,30, &rsize) == 0) {
        // printf("rsiez = %d",rsize);
        printf("%-30s%-15s\n", name, number);
    } else {
        printf("no name match!\n");
    }
}

void editPhoneByNumber(BTA *root) {
    char name[30], number[20];
    int rsize;

    printf("enter the number: ");
    scanf("%[^\n]",number); scanf("%*c");

    if(btsel(root, number,name,30, &rsize) == 0) {
        // printf("rsiez = %d",rsize);
        printf("edit the name: ");
        scanf("%[^\n]",name); scanf("%*c");
        btupd(root,number,name,30);
    } else {
        printf("no name match!\n");
    }
}

void delete(BTA *root) {
    char name[30], number[20];

    printf("enter the number: ");
    scanf("%[^\n]",number); scanf("%*c");

    if(btdel(root,number) == 0) {
        printf("delete ok!\n");
    } else {
        printf("no phone match!\n");
    }
}

void print(BTA *root)
{
    BTint i;
    char number[20];
    char name[30];
    int rsize;
    btpos(root, ZSTART);
    while(bnxtky(root, number, &i) == 0) {
        btsel(root, number, name, 30, &rsize);
        printf("%-30s%-15s\n", name, number);
    }
}

int main() {
    int ls;
    BTA *root;

    btinit();
    
    root = btcrt("contact", 0, FALSE);

    do {
        printf("\n------menu-------\n");
        printf("1.Insert from file\n2.Insert from keyboard\n3.Search by name\n4.Search by number\n5.Edit\n6.Delete\n7.List all contact\n0.Quit\nYou choose: ");
        scanf("%d", &ls); scanf("%*c");

        switch(ls) {
            case 1: readFile(root); break;
            case 2: insert(root); break;
            case 3: searchByName(root); break;
            case 4: searchByNumber(root); break;
            case 5: editPhoneByNumber(root); break;
            case 6: delete(root); break;
            case 7: print(root); break;
            case 0: break;
            default: printf("Invalid input!\n");
        }
    } while(ls != 0);

    btcls(root);
    return 0;
}
