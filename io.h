#include "huffman.h"

#define NULL_CHILD_MARKER "NC\n"
#define NULL_CHILD_MARKER_SIZE 3

void write_node( FILE*, hfnode* );
void write_tree( FILE* fp, hfnode* t );

hfnode read_node( FILE* );
void read_tree_helper( FILE*, hfnode** );
hfnode* read_tree( FILE* );

void read_into_buffer( FILE*, array_char*, char );
char* peek( FILE* fp, int n );
int is_equal_str(char*, char*, int);

array_char* read_decoded_file( FILE* );
void write_decoded_file( FILE*, array_char* );

encoded_file read_encoded_file( FILE* fp );
void write_encoded_file( FILE* fp, encoded_file f );
