#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Label_vector.h"

#define VECTOR_INITIAL_LENGTH 25

static inline int new_length(int length){
    return 2*length;
}


Label_vector * Label_vector_construct(){
    Label_vector * res = malloc(sizeof(Label_vector));
    res->length = VECTOR_INITIAL_LENGTH;
    res->count = 0;
    res->arr = malloc(sizeof(Label *) * res->length);
    return res;
}

Label * Label_vector_create_label(Label_vector * vect, char * name, int address, int line_no){

    Label * label = malloc(sizeof(Label));
    strcpy(label->name, name);
    label->address = address;
    label->line_no = line_no;

    if (vect->count == vect->length){
        vect->length = new_length(vect->length);
        vect->arr = realloc(vect->arr, sizeof(Label *) * vect->length);
    }

    vect->arr[vect->count] = label;
    vect->count++;

    return label;
}

int Label_vector_search(Label_vector * vect, char * name){

    int i = 0;
    bool found = 0;
    while (! found && i < vect->count){
        // arr is an array of Label * (aka Label **), can use [] on it
        found = (! strcmp( vect->arr[i]->name, name) );
        i++;
    }

    return found ? i - 1 : -1;
}

void Label_deconstruct(Label_vector * vect){

    int i = 0;
    while (i < vect->count){
        free(vect->arr[i]);
    }

    free(vect);
}