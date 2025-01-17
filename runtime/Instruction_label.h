#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Instruction {
    unsigned char opcode;
    signed short  operand;
} Instruction;

typedef struct Instruction_vector {
    Instruction * arr;
    size_t size;
    size_t count;
} Instruction_vector ;

void Instruction_vector_init(Instruction_vector * v);

void Instruction_vector_free(Instruction_vector * v);

void Instruction_vector_append(Instruction_vector * v, unsigned char opcode, signed short operand);

#endif