#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "qsort.h"

#define MAX 100

int intCmp(void const *x, void const *y)
{
	if (x == y)
		return 0;
	else if (x > y)
		return 1;
	else
		return -1;
}

void display(int a[], int size)
{
	int i;
	for (i = 0; i < size; ++i)
		printf("%d ", a[i]);
}

int main() {
	int a[MAX];
	time_t start, end;
	srand(time(NULL));
	for(int i = 0; i < MAX; ++i)
		a[i] = rand() % 10;
	display(a, MAX);
	start = time(NULL);
	qsort2Ways(a, 0, MAX);
	end = time(NULL);
	display(a, MAX);
	printf("Time difference 1: %f\n", difftime(end, start));

	// start = time(NULL);
	// quick3Ways(b, 0, MAX);
	// end = time(NULL);
	// // display(a, MAX);
	// printf("Time difference 2: %f\n", difftime(end, start));
/*
	start = time(NULL);
	qsort(a, MAX, sizeof(int), intCmp);
	end = time(NULL);
	// display(a, MAX);
	printf("Time difference 3: %f\n", difftime(end, start));*/
	return 0;
}
