#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

typedef struct {

    int response_code;
    int line_no;
    char src_line[100];

} Assembly_response ;

Assembly_response assemble(FILE * src, FILE * output, Assembly_response * response);

void print_response(Assembly_response * response);

#endif