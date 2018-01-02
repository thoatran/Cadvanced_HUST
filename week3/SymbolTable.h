#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 100
#define INCREMENTAL_SIZE 10

typedef struct {
	void *key;
	void *value;
} Entry;

typedef struct {
	Entry *entries;
	int size;
	int total;
	Entry (*makeNode)(void *, void *);
	int (*compare)(void *, void *);
} SymbolTable;

SymbolTable createSymbolTable(Entry (*makeNode)(void *, void *), int(*compare)(void *, void*));
void dropSymbolTable(SymbolTable *tab);
void addEntry(void *key, void *value, SymbolTable *book);
Entry *getEntry(void *key, SymbolTable book);

SymbolTable createSymbolTable(Entry (*makeNode)(void *, void *), int compare(void *, void *)) {
	SymbolTable res;
	res.entries = (Entry *)malloc(sizeof(Entry) * res.size);
	res.size = INITIAL_SIZE;
	res.total = 0;
	res.makeNode = makeNode;
	res.compare = compare;
	return res;
}

void dropSymbolTable(SymbolTable *tab) {
	if(tab == NULL)
		exit(1);
	if(tab->entries != NULL) {
		Entry *to_free_entry = tab->entries;
		int i = 0;
		while(i < tab.entries) {
			free(to_free_entry->key);
			free(to_free_entry->value);
			to_free_entry++;
			i++;
		}
		free(tab->entries);
	}
	tab->total = 0;
	tab->size = INITIAL_SIZE;
	tab->entries = NULL;
}

void addEntry(void *key, void *value, SymbolTable *book) {
	if(key == NULL || book == NULL)
		exit(1);
}