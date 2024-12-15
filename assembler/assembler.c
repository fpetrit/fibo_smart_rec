#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "preprocessor_consts.h"
#include "Label_vector.h"
#include "Error_vector.h"

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
 * must check all the constant limits
 * 
 * is syntax correct ?
 * 
 * if opstring is jmp, jnz, call, is the operand a label and does the label exists
 * we suppose that we cannot reference a label that has not been defined in the previous lines
 */
static unsigned char get_from_line(Error_vector ** errors, char * src_line, char * label, char * opstring, signed short * operand);

static void check_labels(Label_vector * labels, Error_vector ** errors);

static void check_opstring_operand(Error_vector ** errors, char * opstring, unsigned short operand);

// print all the line-level errors
// check and print file-level errors line (e.g. labels, constant limits)
// constructs a Label_arr structure and check its coherence
// for the moment the caller is responsible for the lables array to be large enough
// replaces the file position indicator at begining of the file
unsigned char parse(FILE * src, Label_vector ** labels){

    char line[SRC_LINE_MAX_LEN + 1];
    unsigned int line_no = 0;
    int line_len = 0;
    char label[LABEL_MAX_CHAR_NO + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    signed short operand;
    unsigned int address = 0;
    char opcode;

    line[0] = '\0';

    Error_vector * errors = Error_vector_construct();

    // empty source file
    if ( feof(src) != 0 )
        Error_vector_create_error(&errors, 1, line_no, line, line_len);

    while ( feof(src) == 0){

        line_no++;

        fgets(line, SRC_LINE_MAX_LEN + 1, src);

        get_from_line(&errors, line, label, opstring, &operand);

        if (opstring)
            address++;

        check_opstring_operand(&errors, opstring, operand);

        if ( label ){
            Label_vector_create_label(labels, label, address);
        }

        
    }

    check_labels(* labels, &errors);
    
    Error_vector_display(errors);
    
    return errors->count;
}

// can call this functions only if there is no error
void assemble(FILE * src, FILE * output, Label_vector ** labels);
