#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "assembler.h"

/**
 * opstring are stored in signed char (1 byte)
 * range [-128, 127]
 * signed to allow an error value opstring_mapping.c (-1)
 * 
 * operands are stored in signed shorts (2 bytes)
 * the representation in memory is two's complement
 * range -2^(n-1) to 2^(n-1) - 1
 * [-32768, 32767] in normal decimal
 * [-8000, 7FFF]    in normal hexadecimal (not two's complement)
 * we need at most 5 characters (strings of length 6) to print it in the hexa output file
 * 
 * the instructions number is less than 500
 * the adress n: jmp <etiq> instruction operand is the difference n - <etiq>
 * a signed short is sufficient
 */

// unsigned char range : [0, 255]
const char * responses_codes[] = {
    "assembling succeeded",    // 0
    
} ;

void print_response(Assembly_response * r){

    fprintf(stderr, "In line %d:\n%s\nError no. %d: %s.", r->line_no, r->src_line, r->response_code, responses_codes[r->response_code]);
    
}

/**
 * the passed string shoud be at least of length:
 * 6 for operands value conversion (signed short)
 */
static void decimal_to_hex_string(signed short n, char s[]){

    bool negative = (n < 0 ) ? 1 : 0;

    // we do not want a two's complement hexa representation
    // cast n to unsigned, storing the sign then use sprintf ?

}

Assembly_response assemble(FILE * src, FILE * output, Assembly_response * response){

}

