#include "jval.h"
#include <time.h>

#define MAX 100
jval *create_array_jval_int(int size, int min, int max) {
	jval *array;
	array = (jval*)malloc(sizeof(jval) * size);
	if(array == NULL){
		printf(" allocate failed\n");
		exit(1);
	}
	int i;
	srand(time(NULL));
	for( i = 0; i < size; i++) {
		array[i].i = min + rand()  % (max - min);
	}
	return array;
}

void printf_array_jval(jval *arr, int size) {
	int i;
	for( i = 0; i < size; i++) {
		printf("%d ", arr[i].i);
	}
	printf("\n");
}

void main() {
	jval *a = create_array_jval_int(MAX, 0, 100);
	printf_array_jval(a, MAX);
	printf("___________________________________\n");
	sort_gen_int(a, 0, MAX - 1);
	printf_array_jval(a, MAX);
}