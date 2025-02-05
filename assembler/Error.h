#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../constants.h"

typedef struct {

    unsigned char err_code;
    unsigned int line_no;
    char line[LINE_MAX_LEN];
    char word[LABEL_MAX_LEN];

} Error ;


// just holding locally declared data to pass it to underlying functions in one argument
typedef struct {

    Error error;
    unsigned int * line_no;
    int * len;
    char * line;
    bool skip;

} Checking_infos ;

extern Checking_infos infos;

// affiche dans un joli format les informations contenues dans la variable globale Checking_infos.Error
void display_err(void);

// fonction qui met à jour la variable globale de assembler.c de type Checking_infos avec le bon err_code
// le programme check en permanence la variable globale Checking_infos.Error.err_code : 0 veut dire pas d'erreur, sinon la valeur c'est
// l'index de la chaîne de l'erreur correspondante dans le tableau des erreurs
void set_error(int err_code, char * word);

#endif