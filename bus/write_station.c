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

int main(int argc, char const *argv[])
{

	FILE* f;
	if((f = fopen("station", "r")) == NULL){
		printf("Cannot read Station file.\n");
		return 0;
	}

	JRB station = make_jrb();
	char s[256];
	int n = 0;
	JRB tmp = NULL;

	while((fgets(s, 256, f)) != NULL){
		s[strlen(s) - 1] = '\0';
		if(jrb_find_str(station, s) == NULL){
			jrb_insert_str(station, strdup(s), new_jval_i(0));
			n++;
		}
	}

	fclose(f);

	f = fopen("STATION_DATA", "w");
	fprintf(f, "%d", n);

	jrb_traverse(tmp, station){
		fprintf(f, "\n%s", tmp->key.s);
	}

	fclose(f);
	/* code */
	return 0;
}