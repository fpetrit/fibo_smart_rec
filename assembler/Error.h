#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../constants.h"

typedef struct {

    unsigned char err_code;
    unsigned int line_no;
    char line[LINE_MAX_LEN];
    char word[LABEL_MAX_LEN];

} Error ;


// just holding locally declared data to pass it to underlying functions in one argument
typedef struct {

    Error error;
    unsigned int * line_no;
    int * len;
    char * line;
    bool skip;

} Cheking_infos ;

void display_err(void);

void set_error(int err_code, char * word);

#endif