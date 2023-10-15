// #include "array.c"
#include "hfnode.h"

declare_array( hfnode );

void heapify( array_hfnode*, int );

hfnode* extract( array_hfnode* );

void change_key( array_hfnode*, int, hfnode );

void insert( array_hfnode*, hfnode );

array_hfnode* build_heap( array_hfnode* );