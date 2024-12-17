#ifndef ERROR_VECTOR
#define ERROR_VECTOR

const char * err_codes[] = {
    "assembling succeeded",                             // 0
    "no data in source file",                           // 1
    "this is not a valid opstring",                     // 2
    "no operand is expected for this opstring",         // 3
    "an operand is expexted for this opstring",         // 4
    "wrong operand type for this opstring",             // 5
    "label has not been declared",                      // 6
    "wrong syntax",                                     // 7
    "please declare the label in front of an opstring", // 8
    "the label cannot be an empty string",              // 9
} ;

typedef struct {

    char err_code;
    unsigned int line_no;
    char * src_line;

} Error ;

typedef struct {

    int length;
    int count;
    Error ** arr;

} Error_vector ;

Error_vector * Error_vector_construct();

Error * Error_vector_create_error(Error_vector ** vect, char err_code, unsigned int line_no, char * src_line, int src_line_len);

void Error_vector_deconstruct(Error_vector * vect);

void Error_vector_display(Error_vector * vect);

#endif