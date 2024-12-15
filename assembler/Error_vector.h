#ifndef ERROR_VECTOR
#define ERROR_VECTOR

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

Error * Error_vector_create_error(Error_vector ** vect, char * err_code, unsigned int line_no, int src_line_len);

void Error_vector_deconstruct(Error_vector * vect);

void Error_vector_display(Error_vector * vect);

#endif