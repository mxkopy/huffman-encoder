#include "io.h"

void write_node( FILE* fp, hfnode* n ){

    fprintf( fp, "%i ", n -> c );
    fprintf( fp, "%f ", n -> f );

    for( int i = 0; i < n -> code -> size; i++ )
    if( BITTEST(n -> code -> data, i) )
    fprintf( fp, "%c", '1' );
    else
    fprintf( fp, "%c", '0' );

    fprintf( fp, "%c", '\n' );

}

void write_tree( FILE* fp, hfnode* t ){

    if( !t ){
        fputs(NULL_CHILD_MARKER, fp);
        return;
    }

    write_node( fp, t );

    write_tree( fp, t -> L );
    write_tree( fp, t -> R );

}

hfnode read_node( FILE* fp ){

    char c;
    float f;

    // We'll use the array_char used for the code field as a buffer.
    // Once we read the code, we'll use it like a code field again. 
    array_char* code = construct_array(char, 1);

    read_into_buffer( fp, code, ' ' );

    c = atoi( code -> data );
    code -> size = 0;

    read_into_buffer( fp, code, ' ' );

    f = atof( code -> data );
    code -> size = 0;

    read_into_buffer(fp, code, '\n');

    // The really cool part about this is that we can bitset the char array we're reading from. 
    for( int i = 0; i < code -> size; i++ ){

        if( code -> data[i] == '1' ) BITSET( code -> data, i );
        else                         BITCLEAR( code -> data, i );

    }

    // The last element in code -> data, counted by code -> size, is a null-terminator, so we want to exclude that from the count. 
    code -> size--;

    return (struct hfnode){ c, f, code, NULL, NULL };

}

void read_tree_helper( FILE* fp, hfnode** root ){

    if( is_equal_str(peek(fp, NULL_CHILD_MARKER_SIZE), NULL_CHILD_MARKER, NULL_CHILD_MARKER_SIZE) ){
        fseek(fp, NULL_CHILD_MARKER_SIZE, SEEK_CUR);
        return;
    }

    *root  = malloc(sizeof(hfnode));
    **root = read_node(fp);

    read_tree_helper(fp, &(*root) -> L);
    read_tree_helper(fp, &(*root) -> R);

}

hfnode* read_tree( FILE* fp ){

    hfnode* t; 
    read_tree_helper( fp, &t );
    return t;

}

void read_into_buffer( FILE* fp, array_char* buf, char delimiter ){

    char n = 0;

    while( n != delimiter ){

        n = fgetc( fp );
        append( buf, n );

    }

    buf -> size--;

}

char* peek( FILE* fp, int n ){

    char* c = malloc( n * sizeof(char) + 1 );

    for( int i = 0; i <= n; i++ ) c[i] = '\0';
    for( int i = 0; i <  n; i++ ) c[i] = fgetc(fp);

    fseek(fp, -n, SEEK_CUR);

    return c;

}

int is_equal_str( char* a, char* b, int len ){

    for(int i = 0; i < len; i++)
    if (a[i] != b[i]) return 0;

    return 1;

}

array_char* read_decoded_file(FILE* fp){

    array_char* str = construct_array(char, 1);

    while( !feof(fp) ) append(str, fgetc(fp));

    str -> size--;

    return str;

}

void write_decoded_file( FILE* fp, array_char* file ){

    for( int i = 0; i < file -> size; i++ ) fputc(file -> data[i], fp);

    fclose(fp);

}

encoded_file read_encoded_file( FILE* fp ){

    hfnode* t = read_tree( fp );

    array_char* buf = construct_array(char, 1);

    read_into_buffer( fp, buf, '\n' );

    buf -> data[ buf -> size ] = '\0';

    int n = atoi( buf -> data );

    buf -> data     = realloc( (void*) buf -> data, BITNSLOTS(n) );
    buf -> size     = n;
    buf -> capacity = BITNSLOTS(n);

    for( int i = 0; i <= BITNSLOTS(n); i++ ) buf -> data[i] = fgetc(fp);

    return (struct encoded_file){ buf, t };

}

void write_encoded_file( FILE* fp, encoded_file f ){

    write_tree(fp, f.tree);

    fprintf(fp, "%i\n", f.code -> size);

    for(int i = 0; i < BITNSLOTS(f.code -> size); i++) fputc(f.code -> data[i], fp);
 
    fclose(fp);

}
