#include <libfdr/jrb.h>
#include <libfdr/dllist.h>
//../../lib/include/libfdr/jrb.h
#include <utility.h>
//../../lib/include/utility.h
//2 headers are taken from ../../lib/include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weighted_graph.h"

#define INFINITY_VALUE 10000000000

typedef struct {
	Graph graph;
	int root;
} HuffmanTree;

typedef struct {
	int size;
	int bits;
} Coding;

JRB makeStatsTree(char *buffer, int size); //return jrb holding frequency of characters
/* make Prior Queue from JRB. stats jrb contain:
key: character
val: frequency
=> prior Queue will have form:
key: frequency
val: character
*/
JRB makePriorQueue(JRB stats);
HuffmanTree makeHuffmanTree(char *buffer, int size);
void addHuffmanChar(int ch, Coding *htable, int *bits, int n);
void createHuffmanTable(HuffmanTree htree, int root, Coding *htable, int level, int *bits);
int getBit(int bits, int offset);
void encode(char ch, Coding *htable, char *huffman, int *nbit);
void compress(char *buffer, int size, char *huffman, int *nbit);
