#include <stdio.h>
#include <stdbool.h>
#include "../preprocessor_consts.h"
#include "Error_vector.h"
#include "Label_vector.h"


bool get_from_line(Error_vector ** errors, char * src_line, int line_no, int line_len, char * label, char * opstring, unsigned char * opcode, signed short * operand){

    bool skip = true;
    
    char tmp[SRC_LINE_MAX_LEN];
    strcpy(tmp, src_line);
    int tmp_len = line_len;
    char * tmp_p = tmp;

    // error or empty values
    opstring[0] = '\0';
    * opcode = 255;
    label[0] = '\0';

    // %n conversion specifier also counts the leading whitespaces and excluded characters in %[ set conv. spec.
    // the white spaces usual skipping is suppressed

    //remove \n, guaranteed not at another place than tmp_len - 1
    if (tmp_len != 0 & tmp_p[tmp_len - 1] == '\n'){
        tmp_p[tmp_len - 1] = '\0';
        tmp_len--;
    }

    // skip white spaces and \t
    while (  tmp_p[0] != '\0' & (tmp_p[0] == '\t' || tmp_p[0] == ' ' ) ){
        tmp_p++;
        // only mmodified locally
        tmp_len--;
    }

    // opstring ? label ?
    if (tmp_len != 0){

        if (tmp_p[tmp_len - 1] == ':'){

            if ( tmp_len == 1 )
                Error_vector_create_error(errors, 9, line_no, src_line, line_len);
            else
                strcpy(label, tmp_p);

        }
        
        else {
            
            strcpy(opstring, tmp_p);
            * opcode = opstring_to_opcode(opstring, (size_t)tmp_len);
            if ( * opcode == 255){
                opstring[0] = '\0';
                * opcode = NULL;
                Error_vector_create_error(errors, 2, line_no, src_line, line_len);
            }    
        }
    }

    // if we have a label we must search for the opstring, set the opstring and opcode
   
    // check the label & no opstring error

    return skip;

}


void check_labels(Label_vector * labels, Error_vector ** errors);

void check_opstring_operand(Error_vector ** errors, char * opstring, unsigned short operand);