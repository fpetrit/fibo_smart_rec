#ifndef ERROR_CHECKING_H
#define ERROR_CHECKING_H

#include "Error_vector.h"
#include "Label_vector.h"

bool get_from_line(Error_vector ** errors, char * src_line, int line_no, int line_len, char * label, char * opstring, unsigned char * opcode, signed short * operand);

void check_labels(Label_vector * labels, Error_vector ** errors);

void check_opstring_operand(Error_vector ** errors, char * opstring, unsigned short operand);

#endif