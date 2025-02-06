/**
 * @file
 * @brief Functions to extract & check the data from a source code line. Used by @ref parse and @ref assemble.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <regex.h>

#include "../constants.h"
#include "assembler.h"
#include "Error.h"
#include "Label_vector.h"
#include "opstring_mapping.h"
#include "error_checking.h"


/// @brief Defined in @ref assembler.c to pass all the extraction buffers to the parse underlying functions.
extern Checking_infos infos;

extern regex_t label_regex;


/**
 * @brief Used by @ref check_opcode_operand to know what operand is expected for each opcode.
 * 
 * 0: no operand
 * 1: signed short operand
 * 2: signed short OR label operand
 * 
 * The data associated to the opcode i is stored at index i of the array
 * 
 * @warning The opcode 99 (halt) is locally remapped, it becomes opcode 14 and correponds to the last element of the array.
 */
static const unsigned char opcode_arg[15] = {1, 0, 1, 0, 1, 2, 2, 2, 0, 1, 1, 1, 1, 0, 0};


/// @brief Remove a newline character from the given string.
static inline void remove_final_new_line(char * s){
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}


/**
 * @brief Reads from the string pointed to by *line_tmp_p, puts the result in word_tmp. line_tmp_p is incremented by the number of bytes read, like a cursor in the read string.
 */
static inline void read_word(char * word_tmp, char ** line_tmp_p, int * word_tmp_len, int * line_tmp_len){

    *line_tmp_len = strlen(*line_tmp_p);

    sscanf(*line_tmp_p, " %s%n", word_tmp, word_tmp_len);

    // if the input *line_tmp_p string is empty, *word_tmp_len is not set to 0 by sscanf
    if (**line_tmp_p == '\0') {
        *word_tmp_len = 0;
        *word_tmp = '\0';
    }

    *line_tmp_p += *word_tmp_len;
    *line_tmp_len -= *word_tmp_len;
}


/**
 * Checks if word_tmp contains a valid label name string, copies it into the externally defined & global @ref Checking_infos variable.
 * @param verif Indicates wether or not to perform a regex check on word_tmp to allow filling @ref Checking_infos::label with it. 
 */
static void extract_label(bool verif, char * word_tmp, int * word_tmp_len){

    if ( word_tmp_len > 0 ){
        
        // label detection trigger
        if (word_tmp[*word_tmp_len - 1] == ':'){

            word_tmp[*word_tmp_len - 1] = '\0';

            // !!! * operator precedence is less than -- suffix operator
            (*word_tmp_len) -- ;

            if ( verif && regexec(&label_regex, word_tmp, 0, NULL, 0) == REG_NOMATCH ){
                set_error(BAD_LABEL_NAME, word_tmp);
            }

            else
                strcpy(infos.label, word_tmp);
        }
    }
}


void extract_line(bool verif) {
        
    char line_tmp[LINE_MAX_LEN];
    char * line_tmp_p = line_tmp;

    strcpy(line_tmp, infos.line);
    int line_tmp_len = infos.len;

    char word_tmp[LABEL_MAX_LEN];
    int word_tmp_len = 0;

    int tmp = 0;

    // reset the global buffers, must be NULL for the function parse & assemble if no input
    *infos.opstring = '\0';
    *infos.label = '\0';
    *infos.operand = '\0';

    remove_final_new_line(line_tmp_p);

    // check the comment in the header
    read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);

    if ( word_tmp_len != 0){

        *infos.label = '\0';

        // do nothing if there is no label detection trigger character ':'
        // if word_tmp contains a label, the final ':' character is removed, word_tmp_len is decremented
        // else these arguments are left unmodified
        extract_label(verif, word_tmp, &word_tmp_len);

        if ( ! infos.skip ) {
            
            // if word_tmp was used to read the label, we must read another word
            if ( *infos.label != '\0' ){
                read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);
            }

            // get the opstring, verifications are made in check_opcode_operand, and in parse where we verified if a corresponding opcode exists
            if (word_tmp_len != 0){
                strcpy(infos.opstring, word_tmp);
            }

            // finally read another word to get the operand
            read_word(word_tmp, &line_tmp_p, &word_tmp_len, &line_tmp_len);

            // this test is really important because word_tmp can contain previous read words if no operand present
            // read_word set word_tmp_len to 0 if nothing is read in line_tmp_p
            if (word_tmp_len != 0){
                strcpy(infos.operand, word_tmp);
            }

            sscanf(line_tmp_p, " %s%n", word_tmp, &tmp);

            if (verif && tmp != 0){
                set_error(WRONG_SYNTAX, line_tmp_p);
            }
        }
    }

    else {
        infos.skip = true;
    }
}


void check_labels(Label_vector * labels)
{
    *infos.line = '\0';

    int i = 0;
    while ( i < labels->count && ! infos.error.err_code) {
        
        // necessary because it is used by set_error to display an error
        infos.line_no = labels->arr[i].line_no;

        // label has never been declared but is referenced, error
        if (labels->arr[i].address == -1){
            set_error(UNDECLARED_LABEL, labels->arr[i].name);
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


void check_opcode_operand(void){

    unsigned char opcode = opstring_to_opcode(infos.opstring, strlen(infos.opstring));
    unsigned char type;

    int operand_len = strlen(infos.operand);

    if ( opcode != 255 ){

        // locally remap halt 99 -> 14
        if ( opcode == 99 )
            opcode = 14;

        type = opcode_arg[opcode];

        switch (type)
        {
        case 0:
            if (operand_len)
                set_error(NO_OPERAND_EXPECTED, infos.operand);
            break;

        case 1:
            if (! operand_len)
                set_error(OPERAND_EXPECTED, "");

            else if ( ! is_signed_short(infos.operand, operand_len) )
                set_error(NOT_A_SIGNED_SHORT, infos.operand);
            
            break;

        case 2:
            if ( ! ( regexec(&label_regex, infos.operand, 0, NULL, 0) == 0 || is_signed_short(infos.operand, operand_len) ) )
                set_error(BAD_OPERAND, infos.operand);
        }
    }

    else {
        set_error(BAD_OPSTRING, infos.opstring);
    }
}