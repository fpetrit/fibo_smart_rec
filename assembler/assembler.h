#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "Label_vector.h"

/** SIGNED / UNSIGNED INTEGERS HEXA CONVERSION
 *
 * the string argument shoud be at least of length:
 * - 5 for operand in hexa
 * - 3 (2 digits, a null byte) for an opcode
 */
static inline void short_to_hex_string(signed short n, char * s){
    sprintf(s, "%4.4hx", n);
}

static inline void uchar_to_hex_string(unsigned char n, char * s){
    sprintf(s, "%2.2hhx", n);
}

static inline void short_to_str(signed short number, char * s){
    sprintf(s, "%d", number);
}

static inline int len(char * s){
    int i = 0;
    while (s[i] != '\0'){
        i++;
    }
    return i;
}

void assemble(FILE * src, FILE * output, Label_vector * labels);

int parse(FILE * src, Label_vector * labels);

#endif