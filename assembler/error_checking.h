#ifndef ERROR_CHECKING_H
#define ERROR_CHECKING_H

#include "Error.h"
#include "Label_vector.h"

bool check_line(Cheking_infos * infos, char * label, char * opstring, char * operand);

void check_labels(Cheking_infos * infos, Label_vector * labels);

bool check_opcode_operand(Cheking_infos * infos, unsigned char opcode, char * operand, int operand_len);

bool is_signed_short(char * s, int len);

#endif