#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define SRC_LINE_MAX_LEN            30
#define SRC_INSTRUCTIONS_MAX_NO     500
#define SRC_LINE_CHAR_MAX_NO        100
#define LABEL_MAX_CHAR_NO           30

typedef struct {

    unsigned char response_code;
    int line_no;
    char * src_line;

} Assembly_response ;

Assembly_response assemble(FILE * src, FILE * output, Assembly_response * response);

void print_response(Assembly_response * response);

#endif