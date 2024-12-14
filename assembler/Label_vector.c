#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Label_vector.h"

#define VECTOR_INITIAL_LENGTH 25

static inline new_length(int length){
    return 2*length;
}


Label_vector * Label_vector_construct(){
    Label_vector * res = malloc(sizeof(Label_vector));
    res->length = VECTOR_INITIAL_LENGTH;
    res->count = 0;
    res->arr = malloc(sizeof(Label *) * res->length);
    return res;
}

Label_vector * Label_vector_create_label(Label_vector * vect, char * name, unsigned int address){

    Label_vector * res = vect;

    Label * label = malloc(sizeof(Label));
    label->name = name;
    label->address = address;

    if (vect->count == vect->length){
        vect->length = new_length(vect->length);
        res = realloc(vect, sizeof(Label *) * vect->length);
    }

    res->count++;
    res->arr[res->count] = label;

    return res;
}

int Label_vector_search(Label_vector * vect, char * name){

    int res;

    int i = 0;
    bool found = 0;
    while (! found && i < vect->count){
        found = (! strcmp(vect->arr[i]->name, name) );
        i++;
    }

    res = (found) ? i - 1 : -1;
}

void Label_deconstruct(Label_vector * vect){

    int i = 0;
    while (i < vect->count){
        free(vect->arr[i]);
    }

    free(vect);
}