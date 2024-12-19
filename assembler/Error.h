#ifndef ERROR_VECTOR
#define ERROR_VECTOR

#include <stdio.h>

const char * err_codes[] = {
    "assembling succeeded",                             // 0
    "no data in source file",                           // 1
    "this is not a valid opstring",                     // 2
    "no operand is expected for this opstring",         // 3
    "an operand is expexted for this opstring",         // 4
    "wrong operand type for this opstring",             // 5
    "label has not been declared",                      // 6
    "wrong syntax",                                     // 7
    "the label cannot be an empty string",              // 9
} ;


typedef struct {

    char err_code;
    unsigned int line_no;
    char * line;

} Error ;


typedef struct {

    // dynamic allocation in the checking function, must free the Error, and the char * line in the Error
    Error ** errors;
    int count;

} Error_array ;


// just holding locally declared data to pass it to underlying functions in one argument
typedef struct {

    Error_array * error_arr;
    int * line_no;
    int * len;
    char * line;

} Cheking_infos ;


void display_err(Error * err){
    fprintf(stderr, "In line %d:\n%s\nError no. %d: %s.\n\n", err->line_no, err->line, err->err_code, err_codes[err->err_code]);
}

void create_error(Cheking_infos * infos, int err_code){
    if (infos->error_arr->count < MAX_ERROR_NO){

        // the Checking_info struct have refs to the current line_no, line, etc...
        // the purpose of the Error struct is to "freeze" these infos

        // filling the Error struct, dynamically allocated
        Error * error = malloc(sizeof(Error));
        error->line = malloc( sizeof(char) * (*(infos->line_no)+1) );
        strcpy(error->line, infos->line);
        error->line_no = *(infos->line_no);
        error->err_code = err_code;
        
        // put it in the error array storage provided by the Cheking_info struct
        infos->error_arr->errors[infos->error_arr->count] = error;

        infos->error_arr->count++;
    }       
}

void free_error_array(Cheking_infos * infos){

    for (int i = 0; i < infos->error_arr->count ; i++){
        // the Error line
        free(infos->error_arr->errors[i]->line);
        // the Error struct
        free(infos->error_arr->errors[i]);
    }
}


void display_errors(Cheking_infos * infos){
    for (int i = 0; i < infos->error_arr->count ; i++){
        display_err(infos->error_arr->errors[i]);
    }
}

#endif