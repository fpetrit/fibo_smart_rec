#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Represents an instruction in its compiled form: an opcode and an operand. 
 */
typedef struct {
    unsigned char opcode;
    signed short  operand;
} Instruction;

typedef struct {
    /// @brief  This is an array of @ref Instruction structures.
    Instruction * arr;

    /// @brief The current maximal size of the internal @ref Instruction_vector::arr "array". An automatic resizing occurs when @ref Instruction_vector::count == @ref Instruction_vector::size.
    size_t size;

    /// @brief The current number of @ref Instruction "instructions" stored in the @ref Instruction_vector::arr.
    size_t count;
} Instruction_vector ;

/// @brief Initialize the internal array and the members variable.
void Instruction_vector_init(Instruction_vector * v);

/// @brief Free the dynamically allocated @ref Instruction::arr memory. 
void Instruction_vector_deconstruct(Instruction_vector * v);

/// @brief Fills a free space in the internal @ref Instruction_vector::arr with the passed data. 
void Instruction_vector_append(Instruction_vector * v, unsigned char opcode, signed short operand);

#endif