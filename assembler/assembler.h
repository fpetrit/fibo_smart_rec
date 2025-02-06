#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "Label_vector.h"
#include "Error.h"

// just holding locally declared data to pass it to underlying functions in one argument
typedef struct {

    Error error;
    bool skip;

    char line[LINE_MAX_LEN + 1];
    unsigned int line_no;
    int len;
    int address;

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char operand[LABEL_MAX_LEN + 1];
    unsigned char opcode;
    
} Checking_infos ;


void assemble(FILE * src, FILE * output, Label_vector * labels);

int parse(FILE * src, Label_vector * labels);

#endif