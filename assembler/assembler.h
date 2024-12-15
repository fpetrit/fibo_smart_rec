#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

void assemble(FILE * src, FILE * output, Label_vector ** labels);

unsigned char parse(FILE * src, Label_vector ** labels);

#endif