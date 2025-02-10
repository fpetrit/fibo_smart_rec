/**
 * @file 
 * @brief Defines an Error structure used to throw, catch and display parsing errors.
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include "../constants.h"

/**
 * Stored in @ref Checking_infos::error and used to "freeze" its buffers to later prints the error.
 */
typedef struct {

    unsigned char err_code;
    unsigned int line_no;

    /**
     * @brief The line that triggered the error.
     * @warning May be empty for some errors that are not triggered immediately after extract the current line data (e.g error no. 4).
     */
    char line[LINE_MAX_LEN + 1];

    /**
     * @brief Additional string buffer to hold the part of @ref Error::line that triggered the error, or another string that highlights where is the error.
     */ 
    char word[LABEL_MAX_LEN + 1];

    char label[LABEL_MAX_LEN + 1];
    char opstring[OPSTRING_MAX_LENGTH + 1];
    char operand[LABEL_MAX_LEN + 1];

} Error ;

/**
 * Enumerators corresponding to indexes in the @ref err_codes array definied in @ref error.c. Improves readability when calling @ref set_error.
 */
typedef enum {

    ASSEMBLING_SUCCESS,
    BAD_OPSTRING,
    NO_OPERAND_EXPECTED,
    OPERAND_EXPECTED,
    UNDECLARED_LABEL,
    WRONG_SYNTAX,
    NOT_A_SIGNED_SHORT,
    BAD_LABEL_NAME,
    LABEL_ALREADY_DEFINED,
    BAD_OPERAND,
    EMPTY_FILE,

} PARSING_ERROR ;

/// @brief Displays the error stored in the globally defined @ref Checking_infos::error variable. 
void display_err(void);

/**
 * @ref Sets a parsing error. Fills the @ref Checking_infos::error buffers.
 * @param word A string that highlights where is the error in the line.
 */
void set_error(PARSING_ERROR err_code, char * word);

#endif