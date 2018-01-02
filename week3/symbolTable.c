#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 100
#define INCREMENTAL_SIZE 10

typedef struct {
  void *key;
  void *value;
}Entry;

typedef struct {
  Entry *entries;
  int size;
  int total;
  Entry (*makeNode)(void *, void *);
  int (*compare)(void *, void *);
}SymbolTable;

SymbolTable createSymbolTable(Entry (*makeNode)(void *, void *), int (*compare)(void*, void *));
void dropSymbolTable(SymbolTable *tab);
Entry *getEntry(void *key, SymbolTable *book);
void addEntry(void *key, void *value, SymbolTable *book);
Entry makePhone(void *key, void *value);
int comparePhone(void *key1, void *key2);

SymbolTable createSymbolTable(Entry (*makeNode)(void *, void *), int(*compare)(void *, void*))
{
	SymbolTable res;
	res.size = INITIAL_SIZE;
	res.total = 0;
	res.makeNode = makeNode;
	res.compare = compare;
	res.entries = (Entry *)malloc(sizeof(Entry) * res.size);
	return res;
}


void dropSymbolTable(SymbolTable *tab)
{
	if (tab == NULL)
		exit(1);
	if (tab->entries != NULL)
	{
		Entry *to_free_entry = tab->entries;
		int i = 0;
		while (i < tab->total)
		{
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

Entry *getEntry(void *key, SymbolTable *book){
  Entry *res = book->entries;
  int i = 0;
  while(i < book->total){
    if(book->compare(res->key, key) == 0)
      return res;
    i++;
    res ++;
  }
  return NULL;
}

void addEntry(void *key, void *value, SymbolTable *book){

	if (key == NULL || book == NULL)
		exit(1);
	if (getEntry(key, book) != NULL)
		return;
	if (book->entries == NULL)
		exit(1);
	if (book->total == book->size)
	{
		(book->size) += INCREMENTAL_SIZE;
		(book->entries) = (Entry *)realloc(book->entries, sizeof(Entry) * (book->size));
		if (book->entries == NULL)
			exit(1);
	}
	Entry *add = book->entries;
	add[book->total] = book->makeNode(key, value);
(book->total)++;
}

Entry makePhone(void *name, void *phone){
  Entry new;
  new.key = strdup((char*)name);
  new.value = malloc(sizeof(long));
  memcpy(new.value, phone, sizeof(long));
  return new;
}

int comparePhone(void *key1, void *key2) {
  return strcmp((char*)key1, (char*)key2);
}

