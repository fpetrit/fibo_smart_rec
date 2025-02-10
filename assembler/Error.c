/**
 * @file
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Error.h"
#include "assembler.h"

extern Checking_infos infos;

/// @brief Stores all the parsing error strings. Their index in the array corresponds to the @ref Error::err_code integer.
static const char * err_codes[] = {
    "assembling succeeded",                                                 // 0
    "this is not a valid opstring",                                         // 1
    "no operand is expected for this opstring",                             // 2
    "an operand is expected for this opstring",                             // 3
    "label has not been declared",                                          // 4
    "wrong syntax",                                                         // 5
    "operand must be a signed short",                                       // 6
    "invalid label name, the regex pattern is ^[_a-zA-Z][_0-9a-zA-Z]*$",    // 7
    "label already defined",                                                // 8
    "operand must be a signed short or a label",                            // 9
    "the source file is empty",                                             // 10
} ;


void display_err(void){
    fprintf(stderr, "In line %d: %s\nError no. %d: %s.\n", infos.error.line_no, infos.error.line, infos.error.err_code, err_codes[infos.error.err_code]);
    if (*infos.error.word)
        fprintf(stderr, "---> %s\n", infos.error.word);
    fprintf(stderr, "\n");
}

void set_error(PARSING_ERROR err_code, char * word){
    infos.skip = true;
    strcpy(infos.error.line, infos.line);
    strcpy(infos.error.word, word);
    infos.error.line_no = infos.line_no;
    infos.error.err_code = err_code;
}
