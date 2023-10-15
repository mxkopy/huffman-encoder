#include "huffman.h"

// Given a char array, assigns a frequency to each possible char value ranging from 0 to 255.
array_float* frequencies( array_char* string ){

    array_float* F = construct_array(float, NUM_POSSIBLE_UNSIGNED_CHAR);
    F -> size = NUM_POSSIBLE_UNSIGNED_CHAR; 

    float* D = F -> data;

    int I[NUM_POSSIBLE_UNSIGNED_CHAR];

    for( int i = 0; i < NUM_POSSIBLE_UNSIGNED_CHAR; i++ ) I[i] = 0;

    for( int i = 0; i < string -> size; i++ ) I[ (int) string -> data[i] ]++;

    for( int i = 0; i < NUM_POSSIBLE_UNSIGNED_CHAR; i++ ) D[i] = (float) I[i] / (float) string -> size;

    return F;

}


// Creates an array of Huffman nodes, where each node contains a character and the frequency that character occurs in a string.
// Characters with a frequency of 0 are skipped. 
// L and R pointers are initialized to NULL. 
array_hfnode* create_nodes( array_char* string ){

    array_float* F  = frequencies( string );

    array_hfnode* H = construct_array(hfnode, 256);

    for( int i = 0; i < F -> size; i++ ){

        if( F -> data[i] > 0.0 ){
            hfnode h = (hfnode) {i, F -> data[i], construct_array(char, 1), NULL, NULL};
            append(H, h);
        }
    }

    free( F -> data );
    free( F );

    return H;

}

// Constructs a Huffman tree as described in CLRS
hfnode* construct_tree( array_hfnode* heap ){

    hfnode* D = heap -> data;
    int n     = heap -> size;

    for( int i = 0; i < n - 1; i++ ){

        hfnode node = { 0, 0, construct_array(char, 1), NULL };

        node.L = extract(heap);
        node.R = extract(heap);

        node.f = node.L -> f + node.R -> f;

        insert(heap, node);

    }

    return extract(heap);

}

// Copies a length of one bitset to another
void copy_bits( char* a, char* b, int length, int a_offset, int b_offset ){

    for( int i = a_offset; i < a_offset + length; i++ )
    if( BITTEST(a, i + a_offset) ) BITSET(b, i + b_offset);
    else                           BITCLEAR(b, i + b_offset);

}


// This function grossly misinterprets the typical meaning of the size and capacity fields in an array_char.
// In this case, size refers to the code length, while capacity refers to the allocated char array in which bit-encoded paths are stored. 
void encode_tree_helper( hfnode* p, hfnode* c, int code_length, int is_right ){

    if( !c ) return;

    array_char* a = p -> code;
    array_char* x = c -> code;

    // Set the size of the node's code to the current code-length
    x -> size = code_length;

    // If the code length exceeds the code's capacity, increase the capacity
    if( BITNSLOTS(code_length) > sizeof(char) * x -> capacity ) increase_capacity(x);

    // Copy the ancestor's code into the child's code
    copy_bits( a -> data, x -> data, code_length - 1, 0, 0 );

    // If the node is a right-child, append a 1 to the code
    if( is_right > 0 ) 
    BITSET( x -> data, code_length - 1 );

    // Else, append a 0 to the code
    else 
    BITCLEAR( x -> data, code_length - 1 );

    encode_tree_helper( c, c -> L, code_length + 1, 0 );
    encode_tree_helper( c, c -> R, code_length + 1, 1 );

}


// Given a Huffman tree, assigns a Huffman code to each node.
// The code is simply the path from the root to the node, where 0 is appended to the code of a left-node, and 1 is appended to the code of a right-node. 
// Only the leaves should be of interest. 
void encode_tree( hfnode* t ){

    encode_tree_helper( t, t -> L, 1, 0 );
    encode_tree_helper( t, t -> R, 1, 1 );

}

// Recursively searches a Huffman tree and returns the matching node or NULL if none exists.
hfnode* search_tree( char c, hfnode* n ){

    if( !n ) return n;

    if( n -> c == c ) return n;

    hfnode* l = search_tree( c, n -> L );
    hfnode* r = search_tree( c, n -> R );

    if( l ) return l;
    else    return r;

}

// Encodes a char array into an array of bit-coded chars using Huffman codes.

// This function grossly misinterprets the typical meaning of the size and capacity fields in an array_char.
// In this case, size refers to the code length, while capacity refers to the allocated char array in which bit-encoded paths are stored. 
encoded_file encode( array_char* string ){

    // The following lines set up the Huffman tree
    array_hfnode* nodes = create_nodes( string );
    build_heap( nodes );
    hfnode* tree = construct_tree( nodes );
    encode_tree( tree );
 
    // Construct a char-array for the encoded file
    array_char* code = construct_array(char, 1);

    // code index
    int s = 0;

    for(int i = 0; i < string -> size; i++){

        hfnode* n = search_tree( string -> data[i], tree );

        if( BITNSLOTS( s + n -> code -> size ) > sizeof(char) * code -> capacity ) increase_capacity( code );

        copy_bits( n -> code -> data, code -> data, n -> code -> size, 0, s );

        s += n -> code -> size;

    }

    code -> size = s;

    for( int i = code -> size; i < code -> capacity; i++ ) BITCLEAR( code -> data, i );
    return (struct encoded_file) { code, tree };

}

// This function grossly misinterprets the typical meaning of the size and capacity fields in an array_char.
// In this case, size refers to the code length, while capacity refers to the allocated char array in which bit-encoded paths are stored.
array_char* decode( encoded_file f ){

    array_char* out = construct_array(char, 1);

    hfnode* n = f.tree;

    for( int i = 0; i <= f.code -> size; i++ ){

        if( !(n -> L || n -> R) ){
            append(out, n -> c);
            n = f.tree;
        }

        if( BITTEST(f.code -> data, i) ) n = n -> R;
        else                             n = n -> L;

    }

    return out;
}

void print_node( hfnode* n ){

    printf("\n");

    if( n -> c > 0 ) printf("%c    ", n -> c );
    else             printf("NULL ",  n -> c );

    printf( "%f ", n -> f );
    printf( "%i ", n -> code -> size );

    for( int i = 0; i < n -> code -> size; i++ ){

        if( BITTEST(n -> code -> data, i) == 0 ) printf( "0" );
        else                                     printf( "1" );

    }

    printf("\n");

}

void print_leaves( hfnode* n ){

    if( !n ) return;

    if( !(n -> L || n -> R) ) print_node( n );

    print_leaves(n -> L);
    print_leaves(n -> R);

}
