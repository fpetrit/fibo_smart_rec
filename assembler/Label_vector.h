#ifndef LABEL_VECTOR_H
#define LABEL_VECTOR_H

#include "../constants.h"

typedef struct {

    char name[LABEL_MAX_LEN + 1];
    int address; // definition du label avec :
    unsigned int line_no; // no ligne vers ou ammene l etiquette    

} Label ;

typedef struct {

    // taille maximale du tableau actuellement disponible
    int length;

    // nombre de Label stockés dans le tableau
    int count;

    // tableau dynamique de pointeurs sur Label
    Label ** arr;

} Label_vector ;

// initialise un vecteur de labels
Label_vector * Label_vector_construct(void);

// créer et ajoute dans le tableau dynamique un nouveau label
Label * Label_vector_create_label(Label_vector * vect, char * name, int address, int line_no);

int Label_vector_search(Label_vector * vect, char * name);

void Label_vector_deconstruct(Label_vector * vect);

#endif