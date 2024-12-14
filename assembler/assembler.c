#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assembler.h>

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
    "no data in source file", // 1
    
} ;

void print_response(Assembly_response * r){

    fprintf(stderr, "In line %d:\n%s\nError no. %d: %s.", r->line_no, r->src_line, r->response_code, responses_codes[r->response_code]);
    
}

/** SIGNED / UNSIGNED INTEGERS HEXA CONVERSION
 *
 * the string argument shoud be at least of length:
 * - 6 (a possible minus sign, 4 digits, a null byte) for operands value conversion (signed short)
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


/** returns a response code, verifiy coherence & syntax, do all the verifications
 * must set label, opcode, operand to NULL if not present in the line
 * 
 * is syntax correct ?
 * 
 * if opstring is jmp, jnz, call, is the operand a label and does the label exists
 * we suppose that we cannot reference a label that has not been defined in the previous lines
 */
static unsigned char get_from_line(char * src_line, char * label, unsigned char * opcode, signed short * operand);

void assemble(FILE * src, FILE * output, Assembly_response * final_response){

    char current_line[SRC_LINE_MAX_LEN + 1];
    char current_label[LABEL_MAX_CHAR_NO + 1];
    unsigned char current_opcode;
    signed short current_operand;

    char hexa_opcode[3];
    char hexa_operand[6];

    unsigned char response_code = 0;

    final_response->line_no = 0;
    final_response->response_code = 1; // no data in source file
    // current_line is copied at final_response->src_line only in case of error

    // at most SRC_LINE_MAX_LEN bytes are read from src (& a null byte is appended)
    fgets(current_line, SRC_LINE_MAX_LEN + 1, src);

    while ( response_code == 0 && feof(src) == 0 ){

        response_code = get_from_line(current_line, current_label, &current_opcode, &current_operand);

        final_response->line_no++;
        // final_response->response_code is already 0
        // no need to copy current_line into final_response

        if ( response_code == 0 ){

            // if label is set, create a label (name + adress) and store it for further use

            // if opstring is jmp (5), jnz (6), or call (7), calculate the hexa_operand (adress difference)

            // if opcode is not NULL, put hexa in output, if hexa_operand is not set, just convert current_operand to hexa
            // if NULL, do nothing this is a blank line

        } else {

            strcpy(final_response->src_line, current_line);
            final_response->response_code = response_code;
            // this is the last iteration, the function returns
        }
    }
}

