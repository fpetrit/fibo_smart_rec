/**
 * @file
 * @brief Header for the simulator 2 main functions: parse and assemble.
 */


#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdbool.h>
#include "Label_vector.h"
#include "Error.h"

/**
 * @brief Groups together several buffers used to read data from the source file and write the hexa file.
 * A single globally defined instance of this structure (declared in @ref assembler.c) is used to pass the data between all the parsing and writing functions.
 */
typedef struct {

    Error error;
    bool skip;

    char line[LINE_MAX_LEN + 1];
    unsigned int line_no;
    int len;
    int address;

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char operand[LABEL_MAX_LEN + 1];
    unsigned char opcode;
    
} Checking_infos ;

/**
 * Parses src and checks for erros. Fills the labels vector and check its consistency (each label has been declared only once, no references to unknown labels).
 * Does not write anything in the ouput hexa file.
 * Stops as soon as an error is encountered.
 * @return An error code corresponding to an error string in the @ref err_codes array in @ref Error.c. 0 means no error.
 */
int parse(FILE * src, Label_vector * labels);

/**
 * @brief Reads the src file without verifying its correctness and write the hexadecimal output file accordingly.
 * @param labels must be a consistent label vector (each contained label has a valid address).
 * @warning Do not call this function unless the @ref parse function returns 0 when called with src as an argument.
 */
void assemble(FILE * src, FILE * output, Label_vector * labels);


#endif