#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Error_vector.h"

#define VECTOR_INITIAL_LENGTH 25

static inline new_length(int length){
    return 2*length;
}

Error_vector * Error_vector_construct(){
    Error_vector * res = malloc(sizeof(Error_vector));
    res->length = VECTOR_INITIAL_LENGTH;
    res->count = 0;
    res->arr = malloc(sizeof(Error *) * res->length);
    return res;
}

Error * Error_vector_create_error(Error_vector ** vect, char err_code, unsigned int line_no, char * src_line, int src_line_len){

    Error * error = malloc(sizeof(Error));
    error->err_code = err_code;
    error->line_no = line_no;
    error->src_line = malloc(sizeof(char) * (src_line_len + 1));
    strcpy(error->src_line, src_line);

    if ((*vect)->count == (*vect)->length){
        (*vect)->length = new_length((*vect)->length);
        *vect = realloc(*vect, sizeof(Error *) * (*vect)->length);
    }

    (*vect)->count++;
    (*vect)->arr[(*vect)->count] = error;

    return error;
}

void Error_vector_deconstruct(Error_vector * vect){

    int i = 0;
    while (i < vect->count){
        free(vect->arr[i]->src_line);
        free(vect->arr[i]);
    }

    free(vect);
}