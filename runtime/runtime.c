#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "runtime.h"
#include "Instruction_label.h"
#include "../constants.h"


const char * errors[] = {
    //each error message down below starts with ": " to get a nice error message

    "", // errcode 0 means no error                                                                             // 0
    "dynamic memory allocation failed",                                                                         // 1
    "memory address out of range",                                                                              // 2
    "stack underflow",                                                                                          // 3
    "stack overflow",                                                                                           // 4
    "the program counter register value is not a valid short, hence it cannot be stored in the stack",          // 5
    "PC out of bounds",                                                                                         // 6
    "infinte loop on the same instruction",                                                                     // 7
    "accessing memory out of range",                                                                            // 8
    "division by zero in op %d",                                                                                // 9
    "unknown operation: op %d",                                                                                 // 10
    "stack underflow in op %d",                                                                                 // 11
};
/*
P
A
Sµ
OUµ
BL
I
ER
LES
*/

// necessary to allocate memory for EMT with init_mp
MP mp = {0, 0, NULL, 0};


// throw a running error by setting mp.error accordingly and printing the message (index errcode in global const 'errors' array)
// "catch" happen in the run function, but displayed instant

// prefix 'static' --> makes a function visible only in the file where it is found
// prefix 'inline' --> makes recurrent calls to the function go faster

static inline void throw_running_error(char prefix[], unsigned char errcode, int data){
    mp.error = errcode;
    fprintf(stderr, "Error: %s ", prefix);   //fprintf(stderr, ...) ensures that error messages are printed immediately, even if the program crashes
    fprintf(stderr, errors[errcode], data);
}


// // allocation dynamique de l'espace mémoire de travail (5000 short)
// short *init_mp(void){
//     short *p = malloc(MP_SUP * sizeof(short));
//     if(!p){
//         throw_running_error("[init_mp]", 1, 0);
//     }
//     return p;
//}

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
    if (mp.SP <= 1) // stack underflow
        throw_running_error("[ipop]", 3, 0);

    else if (mp.EMT[mp.SP - 1] < 0 || mp.EMT[mp.SP - 1] >= MP_SUP) {    // invalid address
        throw_running_error("[ipop]", 2, 0);
    }

    else {    
        short n = mp.EMT[mp.SP - 1];
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
    // if (((mp.PC + adr) >= v->count) || ((mp.PC + adr) < 0)){ // faut initialiser ici une variable globale representant le vecteur d'instructions pour que ca marche
    //     throw_running_error("[jmp]", 6);
    // }
    if (adr == -1){    // infinite loop on the same instruction
        throw_running_error("[jmp]", 7, 0);
    }
    else {  
        mp.PC += adr;
        
    }
}



void jnz(short adr){          /*Faire attention au cas ou PC sort de l'intervalle permis*/
    if (mp.SP <= 0) {  // adress(es) out of bounds
        throw_running_error("[jnz]", 3, 0);
    }
    else {
        mp.SP--;
        if (mp.EMT[mp.SP]){
            // if (((mp.PC + adr) >= v->count) || ((mp.PC + adr) < 0)){ // faut initialiser ici une variable globale representant le vecteur d'instructions pour que ca marche
            //     throw_running_error("[jnz]", 6);
            // }
            if (adr == -1){    // infinite loop on the same instruction
                throw_running_error("[jnz]", 7, 0);
            }
            else {  
                mp.PC += adr;

            }
        }
    
    }
}



void call(short adr) {
    if (mp.SP >= MP_SUP){
        throw_running_error("[call]", 4, 0);
    }
    // Empiler PC sur la pile
    mp.EMT[mp.SP++] = mp.PC; //on empile la valeur de l'instruction apres le call           
    // Ajouter adr au registre PC
    if (adr == - 1){    // infinite loop on the same instruction
        throw_running_error("[call]", 7, 0);
    }
    else mp.PC += adr;   /*on pourrait afficher un warning puisque PC sera bcp trop en dehors de la Pile mais sa fera trop de warnings si on effectue la tache bcp trop de fois*/

}



void ret(short x){
    // if (mp.PC == v->count){ // Pc is pointing to smth out of bounds
    //     throw_running_error("[ret]", 8);
    // }
    if (mp.SP < 1) {   // accessing address(es) out of bounds
        throw_running_error("[ret]", 3, 0);
    }
    mp.SP--;
    mp.PC = mp.EMT[mp.SP];
}

void read(short x){

    if ( 0 <= x && x < MP_SUP){
        long input ;
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

void op(short i) {  // ATTENTION TRAVAILLER LE CAS OU Y A QUE UN ELEMENT DANS LE STACKKK et divsion par 0
    if (mp.SP < 2) {   // accessing address(es) out of bounds
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
                else {
                    mp.EMT[mp.SP - 1] = ~mp.EMT[mp.SP - 1];
                    
                }
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
                else {
                    mp.EMT[mp.SP - 1] = -mp.EMT[mp.SP - 1];
                    
                }
                break;
            
            default:
                throw_running_error("[op]", 10, i);
                break;
        }
    }
}

void rnd(short x){      //ATTENTION. Faut etre sur que ce ne'est pas l'utilisateur qui rentre de valeur ici, sinon faut changer le code pour qu'il ne puisse uniquement entrer un truc de valide
    if (mp.SP < 0){ // Out of bounds (case almost impossible to happen)
        throw_running_error("[rnd]", 2, 0);
    }
    else if (mp.SP >= MP_SUP){  // stack overflow
        throw_running_error("[rnd]", 4, 0);
    }
    else {
        // Generating the random number between 0 and x-1
        short random_value = rand() % x;

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

/*ptet c mieux dinitilaiser dans la struct de MP PC et SP en tant que shorts*/
// int main(short){
//     mp.EMT = init_mp();  /*Allouer la memoire dynamique pour l espace memoire de stockage*/
//     mp.EMT[0] = 0;
//     mp.EMT[1] = 2;
//     mp.EMT[2] = 4;
//     mp.EMT[3] = 6;
//     mp.EMT[4] = 8;

//     printf("%d\n", mp.SP);
//     printf("%d\n", mp.PC);

//     push(2);
//     printf("%d\n%d\n",mp.SP, mp.PC);
//     printf("%d\n", mp.EMT[0]);
//     //jmp(2);
//     //printf("%d\n%d\n", mp.SP, mp.PC);
//     //pop(4999);
//     //printf("%d\n", mp.EMT[4999]);
//     //pop(4998);
//     //printf("%d\n", mp.EMT[4998]);
//     //pop(4997);
//     mp.SP = 1;
//     op(0);
// }


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


void run(FILE * hexa){

    short memory[MP_SUP] = {0};
    mp.EMT = memory;
    mp.PC = 0;
    mp.SP = 0;

    Instruction_vector instructions;
    //Instruction_vector_init(&instructions);

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
        // the index in this array of the Instruction is its ADDRESS (line_no - 1, in the hexa file)
        // if the vector is full, the available size automatically increases (vector)
        Instruction_vector_append(&instructions, opcode, operand);
    }

    // running the instructions
    // warning: possibly an ininite loop if there is no halt in instructions
    // other program ending condition ?

    // while the instruction is not halt, and address in the right range
    while ( 0 <= mp.PC && mp.PC < instructions.count && instructions.arr[mp.PC].opcode != 99 && ! mp.error){

        opcode = instructions.arr[mp.PC].opcode;
        operand = instructions.arr[mp.PC].operand;
        mp.PC++;

        // PC is managed by the mp_functions functions individually
        // instruction execution
        mp_functions[opcode](operand);

    }

    // must call this function because of malloc call during init
    Instruction_vector_deconstruct(&instructions);
}