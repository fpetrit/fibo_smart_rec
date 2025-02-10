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
    "", // errcode 0 means no error                                                                                 // 0
    "dynamic memory allocation failed",                                                                             // 1
    "memory address out of range",                                                                                  // 2
    "stack underflow",                                                                                              // 3
    "stack overflow",                                                                                               // 4
    "the program counter register value is not a valid short, hence it cannot be stored in the stack",              // 5
    "PC out of bounds",                                                                                             // 6
    "infinte loop on the same instruction",                                                                         // 7
    "accessing memory out of range",                                                                                // 8
    "division by zero in op %d",                                                                                    // 9
    "unknown operation: op %d",                                                                                     // 10
    "stack underflow in op %d",                                                                                     // 11
    "the process exited because it ran out of instructions (no halt) or because of a jump to an illegal address (no. %d)",  // 12
};

MP mp = {0, 0, NULL, 0};

// keyword 'static' --> makes a function visible only in the file where it is found
// keyword 'inline' --> replaces the the function calls by the function's body (faster execution, no new stack frames, no jumps)

/**
 * @brief Throws a runtime error by setting the @ref MP::error error code.
 * This integer is used as an idex in the @ref errors global array to retrive the correspondig error string. 
 * Displays the error immediately even if it is caught in loop of the @ref run function.
 */
static inline void throw_running_error(char prefix[], unsigned char errcode, int data){
    mp.error = errcode;
    fprintf(stderr, "Error: %s ", prefix);
    fprintf(stderr, errors[errcode], data);
    fprintf(stderr, "\n");
}

void pop(short x) {      
    if (x < 0 || x >= MP_SUP){  // invalid address
        throw_running_error("[pop]", 2, 0);
    }
    else if (mp.SP <= 0) {  // stack underflow
        throw_running_error("[pop]", 3, 0);
    }
    else {
        mp.SP--;
        mp.EMT[x] = mp.EMT[mp.SP];
    }
}

void ipop(short x){
    short n = mp.EMT[mp.SP - 1];
    if (mp.SP <= 1) // stack underflow
        throw_running_error("[ipop]", 3, 0);

    else if (n < 0 || n >= MP_SUP) {    // invalid address
        throw_running_error("[ipop]", 2, 0);
    }

    else {    
        mp.EMT[n] = mp.EMT[mp.SP - 2];
        mp.SP -= 2;
    }
}

void push(short x){
    if (mp.SP >= MP_SUP){   // stack overflow
        throw_running_error("[push]", 4, 0);
    }
    else if (x < 0 || x >= MP_SUP) {    //  invalid address
        throw_running_error("[push]", 2, 0);
    }
    else {
        mp.EMT[mp.SP] = mp.EMT[x];
        mp.SP++;      
    }
}

void ipush(short x){
    if (mp.EMT[mp.SP - 1] < 0 || mp.EMT[mp.SP - 1] >= MP_SUP){  // invalid address
        throw_running_error("[ipush]", 2, 0);
    }
    else {   
        short n = mp.EMT[mp.SP - 1];  
        mp.EMT[mp.SP - 1] = mp.EMT[n];       //SP is not decremented as changes are direclty made to mp.EMT[SP - 1]  
    }
}

void push_(short i){
    if (mp.SP >= MP_SUP){   // stack overflow
        throw_running_error("[push# i]", 4, 0);
    }
    else {
        mp.EMT[mp.SP] = i;
        mp.SP++;       
    }
}

void jmp(short adr){
    if (adr == -1){    // infinite loop on the same instruction
        throw_running_error("[jmp]", 7, 0);
    }
    else mp.PC += adr;    
}

void jnz(short adr){
    if (mp.SP <= 0) {  // adress(es) out of bounds
        throw_running_error("[jnz]", 3, 0);
    }
    else {
        mp.SP--;
        if (mp.EMT[mp.SP]){
            if (adr == -1){    // infinite loop on the same instruction
                throw_running_error("[jnz]", 7, 0);
            }
            else mp.PC += adr;
        }
    }
}

void call(short adr) {
    if (mp.SP >= MP_SUP){   // stack overflow
        throw_running_error("[call]", 4, 0);
    }
    // stack PC on the stack
    mp.EMT[mp.SP++] = mp.PC;     
    if (adr == - 1){    // infinite loop on the same instruction
        throw_running_error("[call]", 7, 0);
    }
    else mp.PC += adr;

}

void ret(short x){
    if (mp.SP <= 0) {   // accessing address(es) out of bounds
        throw_running_error("[ret]", 3, 0);
    }
    mp.SP--;
    mp.PC = mp.EMT[mp.SP];
}

void read(short x){
    if ( 0 <= x && x < MP_SUP){
        long input = 0;
        printf("Enter a short: ");
        scanf("%ld", &input);
        while (getchar() != '\n');
        short a = input;
        if ( a != input ){
            printf("Warning: the value you entered is not a short. It won't be stored as is.\n");
        }
        mp.EMT[x] = a;
    }
    else    // accessing address out of range
        throw_running_error("[read]", 2, 0);
}

void write(short x){
    if (0 <= x && x < MP_SUP) {
        printf("Value at address %d: %hd\n", x, mp.EMT[x]);       
    }
    else throw_running_error("[write]", 2, 0); // accessing address out of range
}

void op(short i) {
    if (mp.SP < 2 && i != 9 && i != 15) {   // accessing address(es) out of bounds
        throw_running_error("[op]", 11, i);
    }
    else {
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
                if (mp.SP < 1) { // accessing address out of bounds
                    throw_running_error("[op]", 11, 9);
                }
                else mp.EMT[mp.SP - 1] = ~mp.EMT[mp.SP - 1];   
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
                if (mp.EMT[mp.SP - 1] == 0){    // division by zero
                    throw_running_error("[op]", 9, 13);
                }
                else {
                    mp.SP--;
                    mp.EMT[mp.SP - 1] /= mp.EMT[mp.SP];    
                }
                break;

            case 14: // Modulo between the two elements at the top of the stack 
                if (mp.EMT[mp.SP - 1] == 0){    // division by zero
                    throw_running_error("[op]", 9, 14);
                }
                else {
                    mp.SP--;
                    mp.EMT[mp.SP - 1] %= mp.EMT[mp.SP];  
                }
                break;

            case 15: // reverse the element at the top of the stack
                if (mp.SP < 1) { // accessing address out of bounds
                    throw_running_error("[op]", 11, 15);
                }
                else mp.EMT[mp.SP - 1] = -mp.EMT[mp.SP - 1];
                break;
            
            default:
                throw_running_error("[op]", 10, i);
                break;
        }
    }
}

 void rnd(short x){
    if (mp.SP >= MP_SUP){ // stack overflow
        throw_running_error("[rnd]", 4, 0);
    }

    else if (x == 0) { // only 0 and -1 are possible values
        short random_value = -rand() % 2; //the remainder keeps the sign of the dividend always
        mp.EMT[mp.SP] = random_value;
        mp.SP++;
    }

    else if (x == -32768){
        printf("Warning: the operand of rnd in the source code minus 1 is not a short. You won't be able to get the value -32769.\n");
        short random_value = -rand() % (-x + 2); //the remainder keeps the sign of the dividend always
        mp.EMT[mp.SP] = random_value;
        mp.SP++;
    }

    else if (x < 0){
        short random_value = -rand() % (-x + 2); //the remainder keeps the sign of the dividend always
        mp.EMT[mp.SP] = random_value;
        mp.SP++;
    }

    else {
        // Generating the random number between 0 and x-1
        short random_value = rand() % x; //the remainder keeps the sign of the dividend always
        mp.EMT[mp.SP] = random_value;
        mp.SP++;
    }
}


void dup(short x){
    if (mp.SP <= 0){    // Empty stack
        throw_running_error("[dup]", 2, 0);
    }
    else if (mp.SP >= MP_SUP){  // stack overflow
        throw_running_error("[dup]", 4, 0);
    }
    else {
        mp.EMT[mp.SP] = mp.EMT[mp.SP - 1];
        mp.SP++;  
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
    ret,
    read,
    write,
    op,
    rnd,
    dup,
};


/**
 * @brief Contains the runtime loop to execute each instructions (loaded in a @ref Instruction_vector). Manages the Prgram Counter (PC).
 * The simulator ram is allocated here on the stack. 
 */
void run(FILE * hexa){

    short memory[MP_SUP] = {0}; // initializing all the memory "cells" to 0
    mp.EMT = memory;
    mp.PC = 0;
    mp.SP = 0;

    Instruction_vector instructions;
    Instruction_vector_init(&instructions);

    // fseek: reset the file cursor at the specified position
    fseek(hexa, 0, SEEK_SET);

    // buffers to extract data from the hexa "machine code"
    unsigned char opcode;
    short operand;
  
    // browsing hexa file to fill the Instruction_vector structure
    while ( ! feof(hexa) ){

        fscanf(hexa, " %hhx %hx", &opcode, &operand);

        // this fills an Instruction structure stored in the instructions.arr array
        // the index in this array of the Instruction is its ADDRESS (line_no - 1, in the hexa file)
        // if the vector is full, the available size automatically increases (vector)
        Instruction_vector_append(&instructions, opcode, operand);
    }

    // while the instruction is not halt, and address is in the right range
    while ( 0 <= mp.PC && mp.PC < instructions.count && instructions.arr[mp.PC].opcode != 99 && ! mp.error){

        opcode = instructions.arr[mp.PC].opcode;
        operand = instructions.arr[mp.PC].operand;
        mp.PC++;

        // instruction execution
        mp_functions[opcode](operand);
    }

    if (! mp.error && instructions.arr[mp.PC].opcode != 99)
        throw_running_error("[runtime]", 12, mp.PC);

    // must call this function because of malloc call during init
    Instruction_vector_deconstruct(&instructions);
}