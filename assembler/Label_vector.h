#ifndef LABEL_VECTOR_H
#define LABEL_VECTOR_H

typedef struct {

    char * name;
    int address;
    unsigned int line_no;

} Label ;

typedef struct {

    int length;
    int count;
    Label ** arr;

} Label_vector ;

Label_vector * Label_vector_construct();

Label * Label_vector_create_label(Label_vector * vect, char * name, int address, int line_no);

int Label_vector_search(Label_vector * vect, char * name);

void Label_vector_deconstruct(Label_vector * vect);

#endif