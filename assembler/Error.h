#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../constants.h"

typedef struct {

    unsigned char err_code;
    unsigned int line_no;
    char line[LINE_MAX_LEN + 1];
    char word[LABEL_MAX_LEN + 1];

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char operand[LABEL_MAX_LEN + 1];

} Error ;

typedef enum {

    ASSEMBLING_SUCCESS,
    BAD_OPSTRING,
    NO_OPERAND_EXPECTED,
    OPERAND_EXPECTED,
    UNDECLARED_LABEL,
    WRONG_SYNTAX,
    NOT_A_SIGNED_SHORT,
    BAD_LABEL_NAME,
    LABEL_ALREADY_DEFINED,
    BAD_OPERAND,

} PARSING_ERROR ;

// affiche dans un joli format les informations contenues dans la variable globale Checking_infos.Error
void display_err(void);

// fonction qui met à jour la variable globale de assembler.c de type Checking_infos avec le bon err_code
// le programme check en permanence la variable globale Checking_infos.Error.err_code : 0 veut dire pas d'erreur, sinon la valeur c'est
// l'index de la chaîne de l'erreur correspondante dans le tableau des erreurs
void set_error(PARSING_ERROR err_code, char * word);

#endif