#include "heap.h"
#include <stdio.h>

// Implements functions related to a heap keyed on an hfnode's float value
// mostly from CLRS

void heapify( array_hfnode* heap, int i ){

    hfnode* D = heap -> data;

    int l = i * 2;
    int r = i * 2 + 1;

    int m;

    if ( l < heap -> size && D[l].f < D[i].f ) m = l;
    else                                       m = i;
    if ( r < heap -> size && D[r].f < D[m].f ) m = r;
    if ( m != i ){

        hfnode T = D[i];
        D[i] = D[m];
        D[m] = T;

        heapify(heap, m);
    }

}

hfnode* extract( array_hfnode* heap ){

    hfnode* D = heap -> data;

    if( heap -> size == 0 ){
        printf("\nERR: Heap underflow, quitting\n");
        exit(0);
    }

    hfnode* m = malloc(sizeof(hfnode));

    *m = D[0];

    D[0] = D[--(heap -> size)];

    heapify( heap, 0 );

    return m;

}

void change_key( array_hfnode* heap, int i_, hfnode k ){

    hfnode* D = heap -> data;

    int i = i_;

    D[i] = k;

    while( i > 0 && D[i/2].f >= D[i].f ){

        hfnode T = D[i];
        D[i] = D[i / 2];
        D[i / 2] = T;

        i /= 2;
    }

}

void insert( array_hfnode* heap, hfnode k ){

    append(heap, k);
    change_key(heap, heap -> size - 1, k);

}

array_hfnode* build_heap( array_hfnode* array ){

    for(int i = array -> size / 2; i >= 0; i--) heapify(array, i);

    return array;

}