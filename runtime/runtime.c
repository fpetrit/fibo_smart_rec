/**
 * @file
 * @brief Contains all the runtime related functions, each opcode instruction implementation and a run function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "runtime.h"
#include "Instruction_vector.h"
#include "../constants.h"

/// @brief Stores the available error strings. Used by the @ref throw_running_error function.
const char * errors[] = {
    "", // errcode 0 means no error                                                                             // 0
    "dynamic memory allocation failed",                                                                         // 1
    "memory address out of range",                                                                              // 2
    "stack underflow",                                                                                          // 3
    "stack overflow",                                                                                           // 4
    "the program counter register value is not a valid short, hence it cannot be stored in the stack",          // 5
    "PC out of bounds",                                                                                         // 6
    "infinte loop on the same instruction",                                                                     // 7
    "accessing memory out of range",                                                                            // 8
    "division by zero",                                                                                         // 9
    "unknown operation",                                                                                        // 10
};

MP mp = {0, 0, NULL, 0};

/**
 * @brief Throws a runtime error by setting the @ref MP::error error code.
 * This integer is used as an idex in the @ref errors global array to retrive the correspondig error string. 
 * Displays the error immediately even if it is caught in loop of the @ref run function.
 */
static inline void throw_running_error(char prefix[], unsigned char errcode){
    mp.error = errcode;
    fprintf(stderr, "Error: %s %s", prefix, errors[errcode]);
}

void pop(short x) {

    // warning: the user may modify the stack

    // invalid address
    
    if (x < 0 || x >= MP_SUP)
        throw_running_error("[pop]", 2);

    // stack underflow
    else if (mp.SP <= 0)
        throw_running_error("[pop]", 3);

    else
        mp.EMT[x] = mp.EMT[-- mp.SP];
}

void ipop(short x){

    // stack underflow
    if (mp.SP < 2)
        throw_running_error("[ipop]", 3);

    else {

        // WARNING: THE USER MAY MODIFY THE STACK
        short n = mp.EMT[mp.SP - 1];

        // invalid address
        if ( n < 0 || n >= MP_SUP)
            throw_running_error("[ipop]", 2);

        else {
            mp.EMT[n] = mp.EMT[mp.SP - 2];
            mp.SP -= 2;
        }
    }
}

void push(short x){

    // stack overflow
    if (mp.SP >= MP_SUP)
        throw_running_error("[push]", 4);

    //  invalid address
    else if (x < 0 || x >= MP_SUP)
        throw_running_error("[push]", 2);

    else
        mp.EMT[mp.SP++] = mp.EMT[x];
}

void ipush(short x){

    // check for underflow error, we need at least one element in the stack
    if ( mp.SP <= 0 )
        throw_running_error("[ipush]", 3);

    else {
        short n = mp.EMT[mp.SP - 1]; 

        // invalid address
        if (n < 0 || n >= MP_SUP)
            throw_running_error("[ipush]", 2);

        else
            //SP is not decremented as changes are direclty made to mp.EMT[SP - 1]
            mp.EMT[mp.SP - 1] = mp.EMT[n];
    }
}

void push_(short x){
    if (mp.SP >= MP_SUP)
        throw_running_error("[push#]", 4);

    else
        mp.EMT[mp.SP++] = x;
}

void jmp(short x){

    // avoid any infinite loop
    if (x == -1)
        throw_running_error("[jmp]", 7);

    else
        mp.PC += x;
}

void jnz(short x){
    if (mp.SP < 1)
        throw_running_error("[jnz]", 3);

    else {
        // pop in all cases
        mp.SP--;
        // avoid any infinite loop
        if (x == -1)    
            throw_running_error("[jnz]", 7);
        else
            mp.PC += mp.EMT[mp.SP] != 0 ? x : 0;    
    }
}

void call(short x) {
    
    // check for stack overflow error
    if (mp.SP >= MP_SUP){
        throw_running_error("[call]", 4);
    }
    
    // push PC onto the stack
    mp.EMT[mp.SP++] = mp.PC;            
    
    // avoid any infinite loop
    if (x == -1){   
        throw_running_error("[call]", 7);
    }

    // if the instruction address x is not specified by a label (labels address validity checked at compile time)
    // a additional verification is done in the run function loop.
    else mp.PC += x;

}

// void ret(short x);

void read(short x){

    if ( 0 <= x && x <= MP_SUP){

        char response = 'y';

        if (x < mp.SP) {
            printf("Warning: you are modifying a value stored in the stack. Continue ? [y/n] ");
            do {
                scanf("%c", &response);
            } while ( response == 'n' || response == 'y' );
        }

        if (response == 'y') {

            long long int input = SHRT_MAX + 1;

            while ( ! ( SHRT_MIN <= input && input <= SHRT_MAX) ) {
                printf("Input (2 bytes max): ");
                scanf("%lli", &input);
            }

            mp.EMT[x] = (short) input;
        }
    
        else printf("Aborted.\n");
    }

    else
        throw_running_error("[read]", 2);
}

void write(short x){
    if (0 <= x && x < MP_SUP)
        printf("Value at address %d: %hd\n", x, mp.EMT[x]);
        
    // invalid address
    else throw_running_error("[write]", 2); 
}

void op(short i) {

    // error checking
    // we are only checking stack underflows
    // stack overflows are detected by the functions incrementing mp.SP

    if ( ( 0 <= i && i <= 14 && mp.SP < 2 ) || ( (i == 9 || i == 15) && mp.SP < 1 ) )
        throw_running_error("[op]", 3);

    else if ( ! (0 <= i && i <= 15) )
        throw_running_error("[op]", 10);
            
    switch (i) {
        case 0: // Equality test

            mp.SP--;
            if (mp.EMT[mp.SP - 1] == mp.EMT[mp.SP]) mp.EMT[mp.SP - 1] = 1;
            else mp.EMT[mp.SP - 1] = 0;
            break;

        case 1: // Inequality test

            mp.SP--;
            if (mp.EMT[mp.SP - 1] != mp.EMT[mp.SP]) mp.EMT[mp.SP - 1] = 1; 
            else mp.EMT[mp.SP - 1] = 0;
            break;

        case 2: // >= test
            mp.SP--;
            if (mp.EMT[mp.SP - 1] >= mp.EMT[mp.SP]) mp.EMT[mp.SP - 1] = 1; 
            else mp.EMT[mp.SP - 1] = 0;
            break;

        case 3: // <= test

            mp.SP--;
            if (mp.EMT[mp.SP - 1] <= mp.EMT[mp.SP]) mp.EMT[mp.SP - 1] = 1; 
            else mp.EMT[mp.SP - 1] = 0;
            break;

        case 4: // > test

            mp.SP--;
            if (mp.EMT[mp.SP - 1] > mp.EMT[mp.SP]) mp.EMT[mp.SP - 1] = 1; 
            else mp.EMT[mp.SP - 1] = 0;
            break;

        case 5: // < test

            mp.SP--;
            if (mp.EMT[mp.SP - 1] < mp.EMT[mp.SP]) mp.EMT[mp.SP - 1] = 1; 
            else mp.EMT[mp.SP - 1] = 0;
            break;

        case 6: // | 

            mp.SP--;
            mp.EMT[mp.SP - 1] |= mp.EMT[mp.SP];
            break;
        
        case 7: // ^ 

            mp.SP--;
            mp.EMT[mp.SP - 1] ^= mp.EMT[mp.SP];
            break;

        case 8: // &

            mp.SP--;
            mp.EMT[mp.SP - 1] &= mp.EMT[mp.SP];
            break;

        case 9: // ~ 

            mp.EMT[mp.SP - 1] = ~mp.EMT[mp.SP - 1];
            break;

        case 10: // add up the two elements at the top of the stack

            mp.SP--;
            mp.EMT[mp.SP - 1] += mp.EMT[mp.SP];
            break;

        case 11: // substract the two elements at the top of the stack 

            mp.SP--;
            mp.EMT[mp.SP - 1] -= mp.EMT[mp.SP];
            break;

        case 12: // multiply the two elements at the top of the stack  

            mp.SP--;
            mp.EMT[mp.SP - 1] *= mp.EMT[mp.SP];
            break;

        case 13: // whole division between the two elements at the top of the stack

            if (mp.EMT[mp.SP - 1] == 0)    // division by zero
                throw_running_error("[op 13]", 9);

            else {
                mp.SP--;
                mp.EMT[mp.SP - 1] /= mp.EMT[mp.SP];
            }
            break;

        case 14: // Modulo between the two elements at the top of the stack 

            if (mp.EMT[mp.SP - 1] == 0)   // division by zero
                throw_running_error("[op 14]", 9);

            else {
                mp.SP--;
                mp.EMT[mp.SP - 1] %= mp.EMT[mp.SP];
            }
            break;

        case 15: // reverse the element at the top of the stack

            mp.EMT[mp.SP - 1] = -mp.EMT[mp.SP - 1]; 
            break;
        
        default:
            break;
    }
}

void rnd(short x){      

    if (mp.SP >= MP_SUP) // stack overflow
        throw_running_error("[rnd]", 4);
    
    else {
        // Generating the random number between 0 and x-1
        short random_value = rand() % x;
        mp.EMT[mp.SP++] = random_value;
    }
}

void dup(short x){

    if (mp.SP >= MP_SUP) // stack overflow
        throw_running_error("[dup]", 4);
    
    else if ( mp.PC < 1 )
        throw_running_error("[dup]", 3);

    else {
        // !!! do not write mp.EMT[mp.SP++] = ... (-Wsequence-point), as the order of operations executions is undefined
        // SP is modified and read in the same statement
        mp.EMT[mp.SP] = mp.EMT[mp.SP - 1];
        mp.PC++;
    }
        
}

const void (*mp_functions[15])(short) = {
    pop,
    ipop,
    push,
    ipush,
    push_,
    jmp,
    jnz,
    call,
    call, // REPLACE WITH RET WHEN IMPLEMENTED
    read,
    write,
    op,
    rnd,
    dup,
};

void run(FILE * hexa){

    short memory[MP_SUP];
    mp.EMT = memory;

    Instruction_vector instructions;
    Instruction_vector_init(&instructions);

    // fseek: reset the file cursor at the specified position
    // SEEK_SET is a constant definied in the standard lib
    // it means the beginning of the file
    // All in all fseek(*FILE, steps+start, start)
    fseek(hexa, 0, SEEK_SET);

    // bufffers to extract data from the hexa "machine code"
    unsigned char opcode;
    short operand;
  
    // browsing hexa file to fill the Instruction_vector structure
    while ( ! feof(hexa) ){

        fscanf(hexa, " %hhx %hx", &opcode, &operand);

        // this fills an Instruction structure stored in the instructions.arr array
        // if the vector is full, the available size automatically increases (vector)
        Instruction_vector_append(&instructions, opcode, operand);
    }

    // running the instructions
    // while the instruction is not halt, and address in the right range
    while ( 0 <= mp.PC && mp.PC < instructions.count && instructions.arr[mp.PC].opcode != 99 && ! mp.error) {

        opcode = instructions.arr[mp.PC].opcode;
        operand = instructions.arr[mp.PC].operand; 

        // instruction execution
        mp_functions[opcode](operand);

        // PC incrmements are managed externally by the run function
        mp.PC ++;
    }

    // must call this function because of malloc call during init
    Instruction_vector_deconstruct(&instructions);
}