#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

void assemble(FILE * src, FILE * output, Label_vector * labels);

int parse(FILE * src, Label_vector * labels);

#endif