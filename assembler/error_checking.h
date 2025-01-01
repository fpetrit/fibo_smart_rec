#ifndef ERROR_CHECKING_H
#define ERROR_CHECKING_H

#include "Error.h"
#include "Label_vector.h"

extern regex_t label_regex;
extern Cheking_infos infos;

void check_line(char * label, char * opstring, char * operand);

void check_labels(Label_vector * labels);

void check_opcode_operand(unsigned char opcode, char * operand, int operand_len);

bool is_signed_short(char * s, int len);

#endif