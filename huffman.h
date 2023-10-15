#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

#include <limits.h>		/* for CHAR_BIT */

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

#define NUM_POSSIBLE_UNSIGNED_CHAR 256

typedef struct encoded_file {

    array_char* code;

    hfnode* tree;

} encoded_file;

array_float* frequencies( array_char* );
array_hfnode* create_nodes( array_char* );
hfnode* construct_tree( array_hfnode* );

void encode_tree_helper( hfnode*, hfnode*, int, int );
void encode_tree( hfnode* );

hfnode* search_tree( char, hfnode* );

encoded_file encode( array_char* );
array_char* decode( encoded_file );

void print_node( hfnode* );
void print_leaves( hfnode* );
