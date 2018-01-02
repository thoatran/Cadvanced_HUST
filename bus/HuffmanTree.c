#include "HuffmanTree.h"

JRB makeStatsTree(char *buffer, int size)
{
	// make jrb to count characters
	JRB stats;
	JRB found; // result of searching
	int val;
	int i;

	stats = make_jrb();

	for(i = 0; i < size; i++){
		found = jrb_find_int(stats, buffer[i]);
		if(!found){
			/* if not found, insert buffer[i] to
			the tree with val as 1 */
			jrb_insert_int(stats, buffer[i], new_jval_i(1));
		}else{
			/* if found,
			increase val of that node by 1 */
			val = jval_i(found->val) + 1;
			found->val = new_jval_i(val);
		} //end if else
	}// end for

	return stats;
}



/* make Prior Queue from JRB. stats jrb contain:
key: character
val: frequency
=> prior Queue will have form:
key: frequency
val: character
*/
JRB makePriorQueue(JRB stats)
{
	JRB tmp;
	JRB priorQ = make_jrb();

	jrb_traverse(tmp, stats){
		jrb_insert_int(priorQ, jval_i(tmp->val), tmp->key);
	}

	return priorQ;

}
/*
priorQ
ptr is used to hold the position of the queue after each operation
count will be 0 for the first dequeue, non zero for later operation
*/

JRB deQueuePrior(JRB priorQ, JRB *ptr, int count) {
	JRB p;
	if(count == 0) {
		count ++;
		*ptr = jrb_first(priorQ);
		return *ptr;
	} else {
		p = *ptr;
		*ptr = jrb_next(p);
		if(p == NULL)
			return NULL;
		return *ptr;
	}
}


/* build huffman tree from a string */
/*
We represent the huffman tree as follow:
the key of each leaves is the ASCII of the character
the key of their parents are the sum of its children
each parent will have two children
*/

HuffmanTree makeHuffman(char *buffer, int size)
{
	JRB stats = makeStatsTree(buffer, size);
	JRB priorQ = makePriorQueue(stats);
	JRB q_pointer = jrb_first(priorQ);
	JRB node1, node2, tmp;
	int parent_id, ch1, ch2, freq1, freq2;
	int count = -1; // used for priorQ. Run it the first time
	HuffmanTree htree = {createGraph(), 0};

	if(buffer == NULL) {
		printf("Error: buffer is null. Cannot create huffman tree.\n");
		exit(1);
	}

	Graph g = htree.graph;

	if(size == 1){
		/* if buffer contain only 1 character
		set root of huffman tree to that character
		set graph of htree to Null for edges and vertices */
		htree.root = buffer[0];
		//printf("%d\n", htree.root);
		htree.graph.edges = NULL;
		htree.graph.vertices = NULL;
		return htree;
	}

	while(q_pointer != jrb_nil(priorQ)){
		/* debug
		jrb_traverse(tmp, priorQ){
			printf("%d\n", jval_i(tmp->key));
		}
		*/
		count++;
		node1 = deQueuePrior(priorQ, &q_pointer, count);
		count++;
		node2 = deQueuePrior(priorQ, &q_pointer, count);

		if(node1 != jrb_nil(priorQ) && node2 != jrb_nil(priorQ)){
			ch1 = jval_i(node1->val);
			ch2 = jval_i(node2->val);
			freq1 = jval_i(node1->key);
			freq2 = jval_i(node2->key);
			// printf("key1 = %d, key2 = %d\n", freq1, freq2);
			// get parent id
			parent_id = ch1 + ch2;
			addEdge(g, parent_id, ch1, 0);
			addEdge(g, parent_id, ch2, 1);
			addVertex(g, parent_id, NULL);
			addVertex(g, ch1, NULL);
			addVertex(g, ch2, NULL);

			// add parent to priorQ
			jrb_insert_int(priorQ, freq1+freq2, new_jval_i(parent_id));
		}

	} // end while

	htree.root = parent_id;

	return htree;

}

/* add an integer array of coding bits to bits in an integer.
ch is the character to be encoded
bits is the integer array of encoded bits of ch
n is the length of the array, or the size of encoded bits
*/

void addHuffmanChar(int ch, Coding *htable, int *bits, int n) {
	int i;
	int code = 0;
	htable[ch].size = n;
	printf("%c: ", ch);
	for(i = 0; i < n; i++) {
		//convert an array of bits into bits in integer
		printf("%d", bits[i]);
		code = code | (bits[i] << i);
	}

	printf("\n");
	htable[ch].bits = code;
}

/*
for the root of the tree, level must be passed as 0.
array bits should be initialized to have at least 20 elements
*/
void createHuffmanTable(HuffmanTree htree, int root, Coding *htable, int level, int *bits) {
	Graph g = htree.graph;
	int adjVertices[2];
	int i;
	if(!g.edges & !g.vertices) {
		htable[htree.root].size = 1;
		htable[htree.root].bits = 0;
		return;

	}

	int n = getAdjacentVertices(g, root, adjVertices);
	if(n == -1){
		addHuffmanChar(root, htable, bits, level);
		return;

	}
	for(i = 0; i < 2; i++){
		bits[level] = (int)getEdgeValue(g, root, adjVertices[i]);
		createHuffmanTable(htree, adjVertices[i], htable, level + 1, bits);
	}
}

/* determine the bit which has offset offset in coding table is 0 or 1 */
int getBit(int bits, int offset)
{
	int test = 1 << offset;
	int result = bits & test;
	if(result){
		return 1;
	}else{
		return 0;
	}
}

void encode(char ch, Coding *htable, char *huffman, int *nbit)
{
	int len = htable[ch].size; // the length of encoded character
	int pos = *nbit;
	int i, element, offset, bit;

	for(i = 0; i < len; i++){
		element = (pos + i) / 7;
		offset = (pos + i) % 7;
		bit = getBit(htable[ch].bits, i);
		if (bit) {
			huffman[element] = huffman[element] | (bit << offset);
		}
		//printf("%d\n", bit);
	}

	*nbit += len;
}


/* compress an array of characters into huffman buffer
nbits will hold the number of bits encoded in huffman buffer
nbits should be initialized to 0 when this function is called
*/

void compress(char *buffer, int size, char *huffman, int *nbit)
{
	/* create coding huffman table */
	HuffmanTree htree = makeHuffman(buffer, size);
	Coding htable[256];
	int bits[100];
	int i;
	*nbit = 0;
	createHuffmanTable(htree, htree.root, htable, 0, bits);

	/* now compress */
	/* init huffman array to 0 */
	for(i = 0; i < size; i++){
		huffman[i] = 0;
	}

	for(i = 0; i < size; i++){
		encode(buffer[i], htable, huffman, nbit);
	}

}