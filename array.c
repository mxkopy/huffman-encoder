#include <stdlib.h>


// The following macros implement a fairly bare-bones generic array. 
// Of the typical array/vector methods, only append and construct_array are given. 
#define declare_array( T ) typedef struct array_##T { \
    int capacity; \
    int size; \
    T* data; \
} array_##T
#define increase_capacity( a ){ \
    a -> capacity *= 2; \
    a -> data = realloc((void*) a -> data, (a -> capacity) * sizeof(typeof(a -> data[0]))); \
}
#define append( a, x ) do { \
    if((a -> size) + 1 > a -> capacity) increase_capacity( a ); \
    a -> data[(a -> size)++] = x; \
} while(0)

#define init_array( T, C, S, D ) (struct array_##T){ .capacity=C, .size=S, .data=D }
#define construct_array( T, C ) ({ \
    array_##T* a = malloc( sizeof( array_##T ) ); \
    *a = (struct array_##T){ .capacity=C, .size=0, .data=malloc(sizeof(T) * C) }; \
    a; \
})

#define indices( index, array ) for( int index = 0; index < array -> size; index++ )

declare_array(char);
declare_array(float);
declare_array(int);
