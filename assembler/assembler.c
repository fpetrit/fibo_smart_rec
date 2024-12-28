#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../constants.h"
#include "assembler.h"
#include "Label_vector.h"
#include "Error.h"
#include "error_checking.h"
#include "opstring_mapping.h"

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
    int operand_len;
    unsigned char opcode;

    unsigned int line_no = 0;
    int address = -1;
    int tmp;
    bool skip;

    // Buffer initialization

    // in case of no data, because line will be printed
    length = 0;
    line[0] = '\0';

    // Checking_info initialization, to pass the line infos + errors easily to underlying checking functions
    // Pointers to only modify the buffers and local variable prevously declared

    Cheking_infos infos;
    infos.error = NULL;
    infos.line = line;
    infos.len = &length;
    infos.line_no = &line_no;

    // empty source file
    if ( feof(src) != 0 )
        set_error(&infos, 1);

    while ( feof(src) == 0 && ! infos.error){

        line_no++;

        // the last non NULL char is '\n' if not eof
        fgets(line, SRC_LINE_MAX_LEN + 1, src);

        length = len(line);

        // GETTING DATA AND CHECKING SYNTAX ERRORS

        // check_line returns a bool indicating if we should check other errors
        // may not be the case because of empty line, syntax error...
        skip = check_line(&infos, label, opstring, operand);

        // if the line is correct and contains data
        if (! skip){

            operand_len = len(operand);

            // if there is an opstring
            if (opstring[0]){
                address++;
                opcode = opstring_to_opcode(opstring, len(opstring));
                skip = check_opcode_operand(&infos, opcode, operand, operand_len);

                if ( ! skip ){

                    // a label is being referenced OR THE OPERAND IS A DECIMAL ADRESS OFFSET
                    if (opcode == 5 || opcode == 6 || opcode == 7){

                        // if the operand is not an adress (signed short) check for label being referenced
                        // else do nothing because the operand is an address, guarenteed by check_opcode_operand()
                        if ( ! is_signed_short(operand, operand_len) ) {

                            tmp = Label_vector_search(labels, operand);

                            // if label has never been declared
                            if ( tmp == -1)
                                Label_vector_create_label(labels, operand, -1, line_no);
                                // if there is no further declaration, adress will remained -1 and it will be detected during check_labels
                            
                            // else:
                            // tmp != -1: label has been referenced but is declared, do nothing
                        }
                    }
                }
            }

            // if there is a label declaration
            if ( ! skip && label[0] ){
                tmp = Label_vector_search(labels, label);

                // label already referenced: must set the adress
                if (tmp != -1){

                    // label has already been defined, else we can assign address
                    if (labels->arr[tmp]->address != -1){
                        set_error(&infos, 11);
                    } else {
                        labels->arr[tmp]->address = address;
                    }
                }

                // label has never been referenced: we create the label and set the adress
                else
                    Label_vector_create_label(labels, label, address, line_no);
            }
        }

    skip = false;

    } // end of while loop

    if (! infos.error)
        check_labels(&infos, labels);

    fseek(src, 0, SEEK_SET);

    if ( infos.error )
        display_err(infos.error);

    // free the Error s in the Error_array
    bool res = infos.error != NULL;
    free_error(&infos);
    
    // can execute assemble only if zero is returned
    return res;
}

// can call this functions only if there is no error
// void assemble(FILE * src, FILE * output, Label_vector ** labels);
