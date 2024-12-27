#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "../constants.h"
#include "Error.h"
#include "Label_vector.h"
#include "opstring_mapping.h"


// 0 no argument
// 1 signed short argument
// 2 signed short argument or label
static const unsigned char opcode_arg[15] = {1, 0, 1, 0, 1, 2, 2, 2, 0, 1, 1, 1, 1, 0, 0};


static void skip_whitespaces_tab(char ** s, int * len){
    while (  **s != '\0' && (**s == '\t' || **s == ' ' ) ){
        *s++;
        *len--;
    }
}

static void remove_final_new_line(char * s, int * len){
    if (*len > 0 && s[*len - 1] == '\n'){
            s[*len - 1] = '\0';
            *len--;
        }
}


static inline void read_word(char * word_tmp, char ** line_tmp_p, int * word_tmp_len, int * line_tmp_len){

    skip_whitespaces_tab(line_tmp_p, line_tmp_len);

    sscanf(word_tmp, "%[^:\t ]%n", *line_tmp_p, word_tmp_len);
    *line_tmp_p += *word_tmp_len;
    *line_tmp_len -= *word_tmp_len;
}


static bool extract_label(Cheking_infos * infos, char * label, char * word_tmp, int word_tmp_len){

    bool skip = false;

    if ( word_tmp_len != 0 ){
        
        // is it a label ?
        if (word_tmp[word_tmp_len - 1] == ':'){
            
            // is the label name at least one character ?
            if ( word_tmp_len > 1){
                word_tmp[word_tmp_len - 1] = '\0';
                strcpy(label, word_tmp);
            }
            // the label name is an empty string
            else {
                set_error(infos, 9);
                skip = true;
            }
        }
    }
    return skip;
}


bool check_line(Cheking_infos * infos, char * label, char * opstring, char * operand) {
    
    bool skip = true;
    
    char line_tmp[SRC_LINE_MAX_LEN];
    strcpy(line_tmp, infos->line);

    // a char * variable can be incremented
    char * line_tmp_p = line_tmp;

    char word_tmp[LABEL_MAX_LEN];
    int word_tmp_len = 0;

    int line_tmp_len = *infos->len;

    // initialize to error/empty values
    opstring[0] = '\0';
    label[0] = '\0';

    remove_final_new_line(line_tmp_p, &line_tmp_len);

    // get the first word, opstring or label ?   
    read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);

    if ( word_tmp_len != 0){

        skip = extract_label(infos, label, word_tmp, word_tmp_len);

        // extract the label if any and true if there is one, we must read more to find label
        if ( ! skip ) {
            
            // if we have read a label, must read more to get opstring
            if ( *label != '\0' ){
                read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);
            }

            // if opstring, copy, else do nothing
            // will be check by check_opcode_operand()
            if (word_tmp_len != 0){
                strcpy(opstring, word_tmp);
            }

            // read the operand (char *)
            read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);
            if (word_tmp_len != 0){
                strcpy(operand, word_tmp);
            }

            skip_whitespaces_tab(&line_tmp_p, &line_tmp_len);

            if (line_tmp_len != 0){
                set_error(infos, 7);
            }
        }
    }
    
    return skip;
}


void check_labels(Cheking_infos * infos, Label_vector * labels)
{
    infos->line = '\0';

    int i = 0;
    while ( i < labels->count && ! infos->error) {

        infos->line_no = &(labels->arr[i]->line_no);

        // il label address have never been set, never been declared, error
        if (labels->arr[i]->address == -1){
            set_error(infos, 6);
        }
    }
}


bool is_signed_short(char * s, int len){

    bool res = true;
    char * endptr;
    long number;

    // strtol do not modify errno in case of success
    // strtol succeeded & errno may be previously set to ERANGE: problem
    errno = ENOENT;
    
    number = strtol(s, &endptr, 10);

    // empty, or do not only contain digits, or long overflow error, or long to signed short cast overflow error
    // we must check ERANGE, not only signed short int limits
    res = (*s != '\0' && endptr == s+len-1 && errno != ERANGE && SHRT_MIN <= number && number <= SHRT_MAX);
    
    return res;
}


bool check_opcode_operand(Cheking_infos * infos, unsigned char opcode, char * operand, int operand_len){

    bool skip = false;
    unsigned char type;

    if ( opcode != 255 ){

        // locally remap halt 99 -> 14
        if ( opcode == 99 )
            opcode = 14;

        type = opcode_arg[opcode];

        switch (type)
        {
        case 0:
            if (operand_len){
                set_error(infos, 3);
                skip = true;
            }
            break;

        case 1:
            if (! operand_len){
                skip = true;
                set_error(infos, 4);
            }
            else if ( ! is_signed_short(operand, operand_len) ){
                skip = true;
                set_error(infos, 10);
            }
            break;

        // case 2: nothing to verify, checked in assemble()
        }
    }

    else {
        set_error(infos, 2);
    }

    return skip;
}