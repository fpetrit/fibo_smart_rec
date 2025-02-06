/**
 * @file
 * @brief A resizable array struct that stores the labels data.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Label_vector.h"

#define VECTOR_INITIAL_LENGTH 25

/// @brief Compute a new larger length from the previous one when the @ref Label_vector::arr is full. 
static inline int new_length(int length){   
    return 2*length + 1;
}

void Label_vector_init(Label_vector * vect){
    vect->arr = malloc(sizeof(Label) * vect->length);
    vect->length = vect->arr ? VECTOR_INITIAL_LENGTH : 0;
    vect->count = 0;
}

Label * Label_vector_create_label(Label_vector * vect, char * name, int address, int line_no){

    if (vect->count == vect->length){
        vect->arr = realloc(vect->arr, sizeof(Label) * new_length(vect->length));
        vect->length = vect->arr ? new_length(vect->length) : 0;
    }

    Label * label = vect->arr ? &vect->arr[vect->count] : NULL;

    if (label) {
        strcpy(label->name, name);
        label->address = address;
        label->line_no = line_no;
        vect->count++;
    }

    return label;
}

int Label_vector_search(Label_vector * vect, char * name){

    int i = 0;
    bool found = 0;
    while (! found && i < vect->count){
        found = (! strcmp( vect->arr[i].name, name) );
        i++;
    }

    return found ? i - 1 : -1;
}

inline void Label_vector_deconstruct(Label_vector * vect){
    free(vect->arr);
}