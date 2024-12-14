#ifndef LABELS_H
#define LABELS_H

#include "../preprocessor_consts.h"

typedef struct {

    char name[LABEL_MAX_CHAR_NO + 1];
    unsigned int address;

} Label ;


typedef struct {

    int length;
    Label * arr;

} Label_arr ;


Label_arr * Label_arr_construct();

bool Label_arr_create_label(Label_arr * arr, char * name, unsigned int address);

int Label_arr_search(Label_arr * arr, char * name);

void Label_deconstruct(Label_arr * arr);

#endif