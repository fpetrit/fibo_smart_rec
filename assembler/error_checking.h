#ifndef ERROR_CHECKING_H
#define ERROR_CHECKING_H

#include "Error.h"
#include "Label_vector.h"

bool check_line(Cheking_infos * infos, char * label, char * opstring, signed short * operand);

void check_labels(Label_vector * labels, Error_array error_arr);

bool check_opcode_operand(Cheking_infos * infos, unsigned char opcode, signed short operand);

#endif