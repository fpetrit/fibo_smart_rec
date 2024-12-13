#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

/** SIGNED / UNSIGNED INTEGERS HEXA CONVERSION
 *
 * the string argument shoud be at least of length:
 * - 6 (a minus sign, 4 digits, a null byte) for operands value conversion (signed short)
 * - 3 (2 digits, a null byte) for opcode conversion (unsigned char)
 */
static void decimal_to_hex_string(signed short n, char s[]){

    // sprintf "x" conversion specifier converts signed integers types to two's complement representation hexadecimal
    // we do not want that
    // cast a signed type to an unsigned type do not keep the correct absolute value
    // we must work with abs() from stdlib to have signed positive numbers
    // abs() supports int argument

    // "%0.4x" format string : flag field: '0' for left padding 0s, precision field: .4 for a 4 digits result

    char sign[2] = "";

    if (n < 0)
        sign[0] = '-';

    sprintf(s, "%s%0.4x", sign, abs( (int)n) );
}

// for opcodes
static void decimal_to_hex_string(unsigned char n, char s[]){

    // sprintf "x" conversion specifier works as we want for unsigned types : no two's complement representation.

    sprintf(s, "0.4x", n);
}

// return 1 if the line syntax is correct, 0 if not
// sets the response code accordingly in any case
// we can work safely with the line after calling parse_line
static bool parse_line(char * src_line, unsigned char * response_code);

// src_line MUST BE VERIFIED WITH parse_line before calling this function
// opcode is filled in, label and operand are possibly empty, return what ?
static bool get_from_line(char * src_line, char * label, unsigned char * opcode, signed short * operand);

Assembly_response assemble(FILE * src, FILE * output, Assembly_response * response);

