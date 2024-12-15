#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "preprocessor_consts.h"
#include "Label_vector.h"
#include "Error_vector.h"
#include "opstring_mapping.h"

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

    char sign[2] = "";

    if (n < 0)
        sign[0] = '-';

    sprintf(s, "%s%0.4x", sign, abs( (int)n) );
}

static void decimal_to_hex_string(unsigned char n, char * s){
    sprintf(s, "0.4x", n);
}

static inline void short_to_str(signed short number, char * s){
    sprintf(s, "%d", number);
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
static unsigned char get_from_line(Error_vector ** errors, char ** src_line, char ** label, char ** opstring, signed short * operand);

static void check_labels(Label_vector * labels, Error_vector ** errors);

static void check_opstring_operand(Error_vector ** errors, char * opstring, unsigned short operand);

unsigned char parse(FILE * src, Label_vector ** labels){

    char line[SRC_LINE_MAX_LEN + 1];
    unsigned int line_no = 0;
    int line_len = 0;

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char opcode;

    signed short operand_short;
    char operand_str[6];

    unsigned int address = 0;
    int tmp;

    line[0] = '\0';

    Error_vector * errors = Error_vector_construct();

    // empty source file
    if ( feof(src) != 0 )
        Error_vector_create_error(&errors, 1, line_no, line, line_len);

    while ( feof(src) == 0){

        line_no++;

        fgets(line, SRC_LINE_MAX_LEN + 1, src);

        // GETTING DATA AND CHECKING SYNTAX ERRORS

        get_from_line(&errors, &line, &label, &opstring, &operand_short);

        // if opstring is NULL, blank line or syntax error, we ignore
        if (opstring){
        
            address++;

            // OPSTRING OPERAND COHERENCE CHECKING

            check_opstring_operand(&errors, opstring, operand_short);

            // LABELS HANDLING

            // there is a label declaration
            if ( label ){
                tmp = Label_vector_search(*labels, label);

                // label already referenced: must set the adress
                if (tmp != -1)
                    (*labels)->arr[tmp]->address = address;
                // label has never been referenced: we create the label and set the adress
                else
                    Label_vector_create_label(labels, label, address);
            }
            
            opcode = opstring_to_opcode(opstring, line_len);

            // a label is being referenced
            if (opcode == 5 || opcode == 6 || opcode == 7){

                // operand_str now contains the label name
                short_to_str(operand_short, operand_str);

                tmp = Label_vector_search(*labels, operand_str);

                // label has never been declared
                if ( tmp == -1)
                    Label_vector_create_label(&labels, operand_str, -1);
                    // if there is no further declaration, adress will remained -1 and it will be detected during check_labels

                // tmp != 1: label has been declared, do nothing
            } 
        }

    } // end of while loop

    check_labels(* labels, &errors);

    fseek(src, 0, SEEK_SET);
    
    Error_vector_display(errors);
    
    return errors->count;
}

// can call this functions only if there is no error
void assemble(FILE * src, FILE * output, Label_vector ** labels);
