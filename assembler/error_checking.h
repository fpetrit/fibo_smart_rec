#ifndef ERROR_CHECKING_H
#define ERROR_CHECKING_H

#include "Error.h"
#include "Label_vector.h"

extern Checking_infos infos;

/**
 * @brief Fills all members of the external @ref Checking_infos variable from the string stored in @ref Checking_infos::line.
 */
void extract_line(bool verif);

/**
 * Checks the consitency of the stored labels adresses after that the function @ref parse have parsed it.
 * 
 * After the call to @ref parse, the referenced but never declared labels will be detected as an error (their address remains -1).
 * In this case, @ref Checking_infos::error will be set.
 * 
 * @warning Resets @ref Checking_infos::line.
 */
void check_labels(Label_vector * labels);

/**
 * @brief Verifies if the combination ( @ref Checking_infos::opcode "opcode", @ref Checking_infos::operand "operand" ) is valid for assembling.
 * 
 * If it is not, @ref Checking_infos::error is set accordingly.
 */
void check_opcode_operand(void);

/**
 * Indicates if the passed string represents an integer that could fit in a signed short variable (2 bytes).
 */
bool is_signed_short(char * s, int len);

#endif