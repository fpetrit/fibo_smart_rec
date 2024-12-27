#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {

    unsigned char err_code;
    unsigned int line_no;
    char * line;

} Error ;


// just holding locally declared data to pass it to underlying functions in one argument
typedef struct {

    Error * error;
    unsigned int * line_no;
    int * len;
    char * line;

} Cheking_infos ;

void display_err(Error * err);

void free_error(Cheking_infos * infos);

void set_error(Cheking_infos * infos, int err_code);

#endif