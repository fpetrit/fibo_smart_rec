#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <regex.h>

#include "../constants.h"
#include "Error.h"
#include "Label_vector.h"
#include "opstring_mapping.h"
#include "error_checking.h"


// 0 no argument
// 1 signed short argument
// 2 signed short argument or label
static const unsigned char opcode_arg[15] = {1, 0, 1, 0, 1, 2, 2, 2, 0, 1, 1, 1, 1, 0, 0};


static void skip_whitespaces_tab(char ** s, int * len){
    while (  **s != '\0' && (**s == '\t' || **s == ' ' ) ){

        // !!! be carefull of operators precedence (segmentation fault)
        (*s)++;
        (*len)--;
    }
}


static void remove_final_new_line(char * s, int * len){
    if (*len > 0 && s[*len - 1] == '\n'){
            s[*len - 1] = '\0';
            (*len)--;
        }
}


static inline void read_word(char * word_tmp, char ** line_tmp_p, int * word_tmp_len, int * line_tmp_len){

    skip_whitespaces_tab(line_tmp_p, line_tmp_len);

    sscanf(*line_tmp_p, "%[^\t ]%n", word_tmp, word_tmp_len);

    // sscanf fais n'importe quoi ? ne met pas *word_tmp_len à 0 si l'input est vide
    if (**line_tmp_p == '\0')
        *word_tmp_len = 0;

    *line_tmp_p += *word_tmp_len;
    *line_tmp_len -= *word_tmp_len;
}


static void extract_label(char * label, char * word_tmp, int word_tmp_len){

    if ( word_tmp_len != 0 ){
        
        // label detection trigger
        if (word_tmp[word_tmp_len - 1] == ':'){

            word_tmp[word_tmp_len - 1] = '\0';

            if ( regexec(&label_regex, word_tmp, 0, NULL, 0) == REG_NOMATCH ){
                set_error(9, word_tmp);
                infos.skip = true;
            }

            else
                strcpy(label, word_tmp);
        }
    }
}


void check_line(char * label, char * opstring, char * operand) {
        
    char line_tmp[LINE_MAX_LEN];
    strcpy(line_tmp, infos.line);

    // a char * variable can be incremented
    char * line_tmp_p = line_tmp;

    char word_tmp[LABEL_MAX_LEN];
    int word_tmp_len = 0;

    int line_tmp_len = *(infos.len);

    // initialize to error/empty values
    // very important
    opstring[0] = '\0';
    label[0] = '\0';
    operand[0] = '\0';

    remove_final_new_line(line_tmp_p, &line_tmp_len);

    // get the first word, opstring or label ?   
    read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);

    if ( word_tmp_len != 0){

        extract_label(label, word_tmp, word_tmp_len);

        if ( ! infos.skip ) {
            
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
            // this test is really important because word_tmp can contain previous read words if no operand present
            // read word set word_tmp_len to 0 if nothing in line_tmp_p
            if (word_tmp_len != 0){
                strcpy(operand, word_tmp);
            }

            skip_whitespaces_tab(&line_tmp_p, &line_tmp_len);

            if (line_tmp_len != 0){
                set_error(7, "");
            }
        }
    }
}


void check_labels(Label_vector * labels)
{
    *infos.line = '\0';

    int i = 0;
    while ( i < labels->count && ! infos.error.err_code) {

        infos.line_no = &(labels->arr[i]->line_no);

        // il label address have never been set, label never been declared but used, error
        if (labels->arr[i]->address == -1){
            set_error(6, labels->arr[i]->name);
        }

        i++;
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
    res = (*s != '\0' && *endptr == '\0' && errno != ERANGE && SHRT_MIN <= number && number <= SHRT_MAX);
    
    return res;
}


void check_opcode_operand(unsigned char opcode, char * operand, int operand_len){

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
                set_error(3, "");
                infos.skip = true;
            }
            break;

        case 1:
            if (! operand_len){
                infos.skip = true;
                set_error(4, "");
            }
            else if ( ! is_signed_short(operand, operand_len) ){
                infos.skip = true;
                set_error(8, operand);
            }
            
            break;

        case 2:
            if ( ! ( regexec(&label_regex, operand, 0, NULL, 0) == 0 || is_signed_short(operand, operand_len) ) ){
                infos.skip = true;
                set_error(12, operand);
            }
        }
    }

    else {
        set_error(2, "");
    }
}