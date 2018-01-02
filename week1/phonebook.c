
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 50
#define PHONE_LEN 50
#define PHONEBOOK_SIZE 500

typedef struct addr_node {
        char name[NAME_LEN];
        char phone[PHONE_LEN];
} addr_t;

char* strUpper(char *s) {
        int i;
        for (i = 0; i < strlen(s); ++i) {
                s[i] = toupper(s[i]);
        }
        return s;
}

int compareContact(const void * contact1, const void * contact2) {
        char name1[50];
        strcpy(name1, ((addr_t*)contact1)->name);
        char name2[50];
        strcpy(name2, ((addr_t*)contact2)->name);
        return (strcmp(strUpper(name1), strUpper(name2)));
}

void strRemoveLast(char *s) {
        if (strlen(s) > 0) {
                s[strlen(s)-1] = '\0';
        }
}

int main(int argc, char *argv[argc]) {
        if (argc != 2) {
                printf("Syntax error. Please use: phonebook <datafile>\n");
                return 1;
        }

        const char * dataFileName = argv[argc-1];

        FILE * fin = fopen(dataFileName, "r");

        int n = 0; //number of contacts
        addr_t contact[PHONEBOOK_SIZE];

        
        char name[NAME_LEN];
        while (!feof(fin)) {
                fgets(name, NAME_LEN, fin);
                if (name[0] == '\n') break;
                // contact = realloc(contact, (++n)*sizeof(addr_t));
                n++;
                strRemoveLast(name);
                strcpy(contact[n-1].name, name);
                fgets(contact[n-1].phone, PHONE_LEN, fin);
                strRemoveLast(contact[n-1].phone);
        }

        printf("PRINT BEFORE SORT\n");
        int i;
        for (i = 0; i < n; ++i) {
                printf("Name: %s\n", contact[i].name);
                printf("Phone: %s\n", contact[i].phone);
        }

        qsort(contact, n, sizeof(addr_t), compareContact);
        
        printf("\n\nPRINT AFTER SORT\n");
        for (i = 0; i < n; ++i) {
                printf("Name: %s\n", contact[i].name);
                printf("Phone: %s\n", contact[i].phone);
        }
        
        return 0;
}