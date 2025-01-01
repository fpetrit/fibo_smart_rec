#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Error.h"


static const char * err_codes[] = {
    "assembling succeeded",                                     // 0
    "no data in source file",                                   // 1  remove
    "this is not a valid opstring",                             // 2
    "no operand is expected for this opstring",                 // 3
    "an operand is expexted for this opstring",                 // 4
    "wrong operand type for this opstring",                     // 5  remove
    "label has not been declared",                              // 6
    "wrong syntax",                                             // 7
    "operand must be a signed short",                           // 8
    "invalid label name",                                       // 9
    "this is not a valid operand",                              // 10
    "label already defined",                                    // 11
    "operand must be a signed short or a label",                // 12
} ;

extern Cheking_infos infos;


void display_err(void){
    fprintf(stderr, "In line %d: %sError no. %d: %s.\n", infos.error.line_no, infos.error.line, infos.error.err_code, err_codes[infos.error.err_code]);
    if (*infos.error.word)
        fprintf(stderr, "---> %s\n", infos.error.word);
    fprintf(stderr, "\n");
}


void set_error(int err_code, char * word){
    strcpy(infos.error.line, infos.line);
    strcpy(infos.error.word, word);
    infos.error.line_no = *infos.line_no;
    infos.error.err_code = err_code;
}