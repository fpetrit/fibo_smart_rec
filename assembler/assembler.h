#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "preprocessor_consts.h"

typedef struct {

    unsigned char response_code;
    int line_no;
    char src_line[SRC_LINE_CHAR_MAX_NO];

} Assembly_response ;


void assemble(FILE * src, FILE * output, Assembly_response * response);

void print_response(Assembly_response * response);

#endif