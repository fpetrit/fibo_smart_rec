#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

#include "../constants.h"
#include "assembler.h"
#include "Label_vector.h"
#include "Error.h"
#include "error_checking.h"
#include "opstring_mapping.h"

/// @brief Globally defined regex_t variable in @ref main.c.
extern regex_t label_regex;

/// @brief See @ref Checking_infos.
Checking_infos infos;

/// @brief Resets all the buffers in @ref Checking_info.  
static inline void init_checking_infos(void){
    infos.error.err_code = 0;
    infos.skip = false;

    *infos.line = '\0';
    infos.line_no = 0;
    infos.len = 0;
    infos.address = -1;

    *infos.label = '\0';
    *infos.opstring = '\0';
    *infos.operand = '\0';
}


int parse(FILE * src, Label_vector * labels){

    init_checking_infos();

    fseek(src, 0, SEEK_SET);

    int tmp;

    while ( ! feof(src) && ! infos.error.err_code) {

        infos.line_no++;

        // the last non NULL char is '\n' if not eof
        fgets(infos.line, LINE_MAX_LEN, src);

        // GETTING DATA AND CHECKING SYNTAX ERRORS

        extract_line(true);

        // if the line is correct and contains data
        if (! infos.skip){

            if (*infos.opstring){

                infos.address++;
                infos.opcode = opstring_to_opcode(infos.opstring, strlen(infos.opstring) );
                check_opcode_operand();

                if ( ! infos.skip ){

                    // a label is being referenced OR THE OPERAND IS A DECIMAL ADRESS OFFSET
                    if (infos.opcode == 5 ||infos.opcode == 6 || infos.opcode == 7){

                        // if the operand is a label, must be registered to check further declaration
                        // else: nothing, checking by check_opcode_operand already done
                        if ( regexec(&label_regex, infos.operand, 0, NULL, 0) == 0 ) {

                            tmp = Label_vector_search(labels, infos.operand);

                            // if label has never been declared
                            if ( tmp == -1)
                                Label_vector_create_label(labels, infos.operand, -1, infos.line_no);
                                // if there is no further declaration, adress will remained -1 and it will be detected during check_labels
                            
                            // else:
                            // tmp != -1: label has been referenced but is declared, do nothing
                        }
                    }
                }
            }

            // if there is a label declaration
            if ( ! infos.skip && *infos.label ){
                tmp = Label_vector_search(labels, infos.label);

                // label already referenced: must set the adress
                if (tmp != -1){

                    // label has already been defined, else we can assign address
                    if (labels->arr[tmp].address != -1){
                        set_error(LABEL_ALREADY_DEFINED, labels->arr[tmp].name);
                        
                    } else {
                        labels->arr[tmp].address = infos.address;
                    }
                }

                // label has never been referenced: we create the label and set the adress
                else
                    Label_vector_create_label(labels, infos.label, infos.address, infos.line_no);
            }
        }

        infos.skip = false;

    } // end of while loop

    if (! infos.error.err_code)
        check_labels(labels);

    if ( infos.error.err_code )
        display_err();
    
    // can execute assemble only if zero is returned
    return infos.error.err_code;
}

// can call this functions only if there is no error
void assemble(FILE * src, FILE * output, Label_vector * labels){

    init_checking_infos();

    fseek(src, 0, SEEK_SET);
    fseek(output, 0, SEEK_SET);

    int operand_len;

    signed short operand_short;
    unsigned char opcode;

    int tmp;
    signed short diff;

    while ( ! feof(src) ){

        infos.skip = false;

        infos.line_no++;

        // read LINE_MAX_LEN bytes max
        fgets(infos.line, LINE_MAX_LEN + 1, src);

        infos.len = strlen(infos.line);

        // extract data without error checkings
        extract_line(false);

        if (! infos.skip){

            operand_len = strlen(infos.operand);

            if ( *infos.opstring ){

                infos.address++;

                if (infos.address)
                    fprintf(output, "\n");

                opcode = opstring_to_opcode(infos.opstring, strlen(infos.opstring));

                fprintf(output, "%2.2hhx", opcode);

                if ( *infos.operand ){

                    if ( is_signed_short(infos.operand, operand_len) ){

                        operand_short = (signed short) strtol(infos.operand, NULL, 10);

                        fprintf(output, " %4.4hx", operand_short);
                    }

                    else if ( regexec(&label_regex, infos.operand, 0, NULL, 0) == 0 ){
                        tmp = Label_vector_search(labels, infos.operand);
                        diff = (signed short) ( labels->arr[tmp].address - infos.address - 1);
                        fprintf(output, " %4.4hx", diff);
                    }
                }
                
                else 
                    fprintf(output, " 0000");
            }
        }
    }
}