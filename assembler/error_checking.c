#include <stdio.h>
#include <stdbool.h>
#include "../preprocessor_consts.h"
#include "Error_vector.h"
#include "Label_vector.h"


static void skip_whitespaces_tab(char ** s, int * len){
    while (  s[0] != '\0' & (s[0] == '\t' || s[0] == ' ' ) ){
        s++;
        *len--;
    }
}

static void remove_final_new_line(char * s, int * len){
    if (len != 0 & s[*len - 1] == '\n'){
            s[*len - 1] = '\0';
            *len--;
        }
}


bool get_from_line(Error_vector ** errors, char * src_line, int line_no, int line_len, char * label, char * opstring, unsigned char * opcode, signed short * operand){

    bool skip = true;
    
    char line_tmp[SRC_LINE_MAX_LEN];
    strcpy(line_tmp, src_line);
    char * line_tmp_p = line_tmp;

    char word_tmp[LABEL_MAX_LEN];
    int word_tmp_len = 0;

    int line_tmp_len = line_len;
    int offset;

    // initialize to error/empty values
    opstring[0] = '\0';
    * opcode = 255;
    label[0] = '\0';

    remove_final_new_line(line_tmp_p, &line_tmp_len); 

    skip_whitespaces_tab(&line_tmp_p, &line_tmp_len);    

    // get the opstring or the label

    sscanf(word_tmp, "%[^:\t ]%n", line_tmp_p, &offset);
    line_tmp_p += offset;
    line_tmp_len -= offset;


    // if we have a label we must search for the opstring, set the opstring and opcode
   
    // check the label & no opstring error

    return skip;

}


void check_labels(Label_vector * labels, Error_vector ** errors);

void check_opstring_operand(Error_vector ** errors, char * opstring, unsigned short operand);