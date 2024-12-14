#ifndef LABEL_VECTOR
#define LABEL_VECTOR

typedef struct {

    char * name;
    unsigned int address;

} Label ;

typedef struct {

    int length;
    int count;
    Label ** arr;

} Label_vector ;

Label_vector * Label_vector_construct();

Label_vector * Label_vector_create_label(Label_vector * vect, char * name, unsigned int address);

int Label_vector_search(Label_vector * vect, char * name);

void Label_deconstruct(Label_vector * vect);

#endif