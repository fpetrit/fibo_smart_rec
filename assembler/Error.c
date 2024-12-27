#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Error.h"


static const char * err_codes[] = {
    "assembling succeeded",                                     // 0
    "no data in source file",                                   // 1
    "this is not a valid opstring",                             // 2
    "no operand is expected for this opstring",                 // 3
    "an operand is expexted for this opstring",                 // 4
    "wrong operand type for this opstring",                     // 5
    "label has not been declared",                              // 6
    "wrong syntax",                                             // 7
    "operand is not in the range of a signed short (2 byes)"    // 8
    "the label cannot be an empty string",                      // 9
    "this is not a valid operand",                              // 10
    "label already defined",                                    // 11
} ;


void display_err(Error * err){
    fprintf(stderr, "In line %d:\n%s\nError no. %d: %s.\n\n", err->line_no, err->line, err->err_code, err_codes[err->err_code]);
}

void free_error(Cheking_infos * infos){
    free(infos->error);
}

void set_error(Cheking_infos * infos, int err_code){

    if (infos->error)
        free_error(infos);
    
    // the Checking_info struct have refs to the current line_no, line, etc...
    // the purpose of the Error struct is to "freeze" these infos

    // is it usefull ? can just print error with the infos struct data, but for labels ?

    // filling the Error struct, dynamically allocated
    Error * new_error = malloc(sizeof(Error));

    new_error->line = malloc( sizeof(char) * *infos->len );
    strcpy(new_error->line, infos->line);
    new_error->line_no = *infos->line_no;
    new_error->err_code = err_code;
        
    infos->error = new_error;
}