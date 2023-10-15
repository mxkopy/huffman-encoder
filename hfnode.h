#include "array.c"

typedef struct hfnode {

    char c;
    float f;

    array_char* code;

    struct hfnode* L;
    struct hfnode* R;

} hfnode;

