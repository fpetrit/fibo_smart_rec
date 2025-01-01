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

regex_t label_regex;
Cheking_infos infos;

void init_label_regex(void){
    regcomp(&label_regex, LABEL_PATTERN, REG_NOSUB);
}


int parse(FILE * src, Label_vector * labels){

    init_label_regex();

    fseek(src, 0, SEEK_SET);

    char line[LINE_MAX_LEN + 1];
    int length;
    unsigned int line_no = 0;
    int address = -1;

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char operand[LABEL_MAX_LEN];
    int operand_len;
    unsigned char opcode;

    int tmp;

    // in case of no data, because line will be printed
    length = 0;
    line[0] = '\0';

    infos.error.err_code = 0;
    infos.skip = false;
    infos.line = line;
    infos.len = &length;
    infos.line_no = &line_no;

    while ( feof(src) == 0 && ! infos.error.err_code){

        line_no++;

        // the last non NULL char is '\n' if not eof
        fgets(line, LINE_MAX_LEN + 1, src);

        length = len(line);

        // GETTING DATA AND CHECKING SYNTAX ERRORS

        extract_line(true, label, opstring, operand);

        // if the line is correct and contains data
        if (! infos.skip){

            operand_len = len(operand);

            if (*opstring){

                address++;
                opcode = opstring_to_opcode(opstring, len(opstring));
                check_opcode_operand(opcode, operand, operand_len);

                if ( ! infos.skip ){

                    // a label is being referenced OR THE OPERAND IS A DECIMAL ADRESS OFFSET
                    if (opcode == 5 || opcode == 6 || opcode == 7){

                        // if the operand is a label, must be registered to check further declaration
                        // else: nothing, checking by check_opcode_operand already done
                        if ( regexec(&label_regex, operand, 0, NULL, 0) == 0 ) {

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
            if ( ! infos.skip && *label ){
                tmp = Label_vector_search(labels, label);

                // label already referenced: must set the adress
                if (tmp != -1){

                    // label has already been defined, else we can assign address
                    if (labels->arr[tmp]->address != -1){
                        set_error(6, labels->arr[tmp]->name);
                    } else {
                        labels->arr[tmp]->address = address;
                    }
                }

                // label has never been referenced: we create the label and set the adress
                else
                    Label_vector_create_label(labels, label, address, line_no);
            }
        }

    infos.skip = false;

    } // end of while loop

    if (! infos.error.err_code)
        check_labels(labels);

    if ( infos.error.err_code )
        display_err();

    // free regex
    regfree(&label_regex);
    
    // can execute assemble only if zero is returned
    return infos.error.err_code;
}

// can call this functions only if there is no error
void assemble(FILE * src, FILE * output, Label_vector * labels){

    init_label_regex();

    fseek(src, 0, SEEK_SET);
    fseek(output, 0, SEEK_SET);

    char line[LINE_MAX_LEN + 1];
    int length;
    unsigned int line_no = 0;
    int address = -1;

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char operand[LABEL_MAX_LEN];
    int operand_len;
    signed short operand_short;
    unsigned char opcode;

    char opcode_hex[3];
    char operand_hex[5];

    length = 0;
    line[0] = '\0';

    int tmp;
    signed short diff;

    infos.line = line;
    infos.len = &length;
    infos.line_no = &line_no;

    while ( ! feof(src) ){

        infos.skip = false;

        line_no++;

        fgets(line, LINE_MAX_LEN + 1, src);

        length = len(line);

        extract_line(false, label, opstring, operand);

        if (! infos.skip){

            operand_len = len(operand);

            if ( *opstring ){

                address++;

                if (address)
                    fprintf(output, "\n");

                opcode = opstring_to_opcode(opstring, len(opstring));
                uchar_to_hex_string(opcode, opcode_hex);

                fprintf(output, "%s", opcode_hex);

                if ( *operand ){

                    if ( is_signed_short(operand, operand_len) ){

                        operand_short = (signed short) strtol(operand, NULL, 10);
                        short_to_hex_string(operand_short, operand_hex);

                        fprintf(output, " %s", operand_hex);
                    }

                    else if ( regexec(&label_regex, operand, 0, NULL, 0) == 0 ){
                        tmp = Label_vector_search(labels, operand);
                        // implicit cast, no error ?
                        diff = labels->arr[tmp]->address - address - 1;
                        short_to_hex_string(diff, operand_hex);
                        fprintf(output, " %s", operand_hex);
                    }
                }
                
                else 
                    fprintf(output, " 0000");
            }
        }
    }

    regfree(&label_regex);

    }
