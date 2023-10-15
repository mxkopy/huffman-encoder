#include "io.h"

int main( int argc, char* argv[] ){

    FILE* r = fopen( argv[1], "r" );
    if( !r ){
        printf("Decoder: File couldn't be opened for reading.\n");
        return 0;
    }

    FILE* w = fopen( argv[2], "w" );
    if( !w ){
        printf("Decoder: File couldn't be opened for writing.\n");
        return 0;
    }

    encoded_file f = read_encoded_file( r );

    array_char* d = decode( f );

    write_decoded_file(w, d);

    return 0;

}
