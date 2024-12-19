#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "constants.h"
#include "Label_vector.h"
#include "Error.h"
#include "error_checking.h"
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
 * [8000, 7FFF]    in two's complement hexadecimal
 * we need at most 4 characters
 * so char str[5]
 * 
 * the instructions number is less than 500
 * the adress n: jmp <etiq> instruction operand is the difference n - <etiq>
 * a signed short is sufficient
 */

// unsigned char range : [0, 255]


/** SIGNED / UNSIGNED INTEGERS HEXA CONVERSION
 *
 * the string argument shoud be at least of length:
 * - 5 for operand in hexa
 * - 3 (2 digits, a null byte) for an opcode
 */
static inline void decimal_to_hex_string(signed short n, char * s){
    sprintf(s, "%0.4x", n);
}

static inline void decimal_to_hex_string(unsigned char n, char * s){
    sprintf(s, "%0.4x", n);
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

/**
 * Goal: extract labels, verifiy labels coherence, verify line syntax, verifiy opstrings, arguments...
 */
int parse(FILE * src, Label_vector * labels){

    // Buffers declaration

    char line[SRC_LINE_MAX_LEN + 1];
    int length;

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];

    // can be a regular decimal operand or a label
    char operand[LABEL_MAX_LEN];
    unsigned char opcode;

    unsigned int line_no = 0;
    unsigned int address = 0;
    signed short operand_short;
    int tmp;
    char ** endptr;


    // Buffer initialization

    // in case of no data, because line will be printed
    length = 0;
    line[0] = '\0';

    // Error handling

    Error errors[MAX_ERROR_NO];
    Error_array err; 
    err.count = 0;
    err.errors = &errors;

    // Checking_info initialization, to pass the line infos + errors easily to underlying checking functions
    // Pointers to only modify the buffers and local variable prevously declared
    // Will not be modified directly

    Cheking_infos infos;
    infos.error_arr = &err;
    infos.line = line;
    infos.len = &length;
    infos.line_no = &line_no;

    // empty source file
    if ( feof(src) != 0 )
        create_error(&infos, 1);

    while ( feof(src) == 0 && infos.error_arr->count < MAX_ERROR_NO){

        line_no++;

        // the last non NULL char is '\n' if not eof
        fgets(line, SRC_LINE_MAX_LEN + 1, src);

        length = len(line);

        // GETTING DATA AND CHECKING SYNTAX ERRORS

        // check_line returns a bool indicating if we should check other errors
        // may not be the case because of empty line, syntax error...
        bool skip = check_line(&infos, label, opstring, operand);

        if (! skip){

            // OPSTRING OPERAND COHERENCE CHECKING

            if (opstring[0]){
                address++;
                opcode = opstring_to_opcode(opstring, len(opstring));
                
                skip = check_opcode_operand(&infos, opcode, operand);

                // is the opsting valid ?
                if ( ! skip ){

                    // a label is being referenced OR THE OPERAND IS A DECIMAL ADRESS OFFSET
                    if (opcode == 5 || opcode == 6 || opcode == 7){

                    operand_short = (signed short)strtol(operand, endptr, 10);

                    // not a valid string to long conversion according to strtol man page
                    // so ckeck for a label being referenced
                    // else do nothing because the operand is an address
                    if (*operand == '\0' || **endptr != '\0'){

                        tmp = Label_vector_search(labels, operand);

                        // if label has never been declared
                        if ( tmp == -1)
                            Label_vector_create_label(labels, operand, -1);
                            // if there is no further declaration, adress will remained -1 and it will be detected during check_labels
                        
                        // else:
                        // tmp != 1: label has been referenced but is declared, do nothing
                    }
                }
            }

            // LABELS HANDLING

            // there is a label declaration
            if ( label[0] ){
                tmp = Label_vector_search(labels, label);

                // label already referenced: must set the adress
                if (tmp != -1)
                    labels->arr[tmp]->address = address;
                // label has never been referenced: we create the label and set the adress
                else
                    Label_vector_create_label(labels, label, address);
            }
        }

        

    } // end of while loop

    check_labels(labels, *infos.error_arr);

    fseek(src, 0, SEEK_SET);

    // free the Error s in the Error_array
    int count = infos.error_arr->count;
    free_error_array(&infos);
    
    // can execute assemble only if zero is returned
    return count;
}

// can call this functions only if there is no error
void assemble(FILE * src, FILE * output, Label_vector ** labels);
