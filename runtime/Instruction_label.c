#include "Instruction_label.h"

#define VECTOR_INITIAL_SIZE 25

void Instruction_vector_init(Instruction_vector * v){
    v->arr = malloc(sizeof(Instruction) * VECTOR_INITIAL_SIZE);
    v->count = 0;
    v->size = VECTOR_INITIAL_SIZE;
}

void Instruction_vector_free(Instruction_vector * v){
    free(v->arr);
    v->count = 0;
    v->size = 0;
}

void Instruction_vector_append(Instruction_vector * v, unsigned char opcode, signed short operand){
    if (v->count >= v->size){
        v->arr = realloc(v->arr, sizeof(Instruction) * v->size * 2 + 1);
        v->size *= 2;
        v->size++;
    }

    v->arr[v->count].opcode = opcode;
    v->arr[v->count].operand = operand;

    v->count++;
}