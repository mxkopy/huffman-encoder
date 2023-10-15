#include "io.h"

int main( int argc, char* argv[] ){

    FILE* r = fopen( argv[1], "r" );
    if( !r ){
        printf("Encoder: File couldn't be opened for reading.\n");
        return 0;
    }

    FILE* w = fopen( argv[2], "w" );
    if( !w ){
        printf("Encoder: File couldn't be opened for writing.\n");
        return 0;
    }

    array_char* str = read_decoded_file( r );

    encoded_file ef = encode( str );

    write_encoded_file(w, ef);

    return 0;

}