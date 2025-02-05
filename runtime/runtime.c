#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "runtime.h"
#include "Instruction_label.h"
#include "../constants.h"


const char * errors[] = {
    //each error message down below starts with ": " to get a nice error message

    "", // errcode 0 means no error                                                                               // 0
    ": dynamic memory allocation failed",                                                                         // 1
    ": memory address out of range",                                                                              // 2
    ": stack underflow",                                                                                          // 3
    ": stack overflow",                                                                                           // 4
    ": the program counter register value is not a valid short, hence it cannot be stored in the stack",          // 5
    ": PC out of bounds"                                                                                          // 6
    ": infinte loop on the same instruction"                                                                      // 7
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
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD

*/

// necessary to allocate memory for EMT with init_mp
MP mp = {0, 0, NULL, 0};


// throw a running error by setting mp.error accordingly and printing the message (index errcode in global const 'errors' array)
// "catch" happen in the run function, but displayed instant

// prefix 'static' --> makes a function visible only in the file where it is found
// prefix 'inline' --> makes recurrent calls to the function go faster

static inline void throw_running_error(char prefix[], unsigned char errcode){
    mp.error = errcode;
    fprintf(stderr, "Error: %s %s", prefix, errors[errcode]);   //fprintf(stderr, ...) ensures that error messages are printed immediately, even if the program crashes
}


// allocation dynamique de l'espace mémoire de travail (5000 short)
short *init_mp(void){
    short *p = malloc(MP_SUP * sizeof(short));
    if(!p){
        throw_running_error("[init_mp]", 1);
    }
    return p;
}


// es tu sur du premier test la deuxieme condition ?    Normalement oui puisque SP <= 0 veut dire que le premier element libre sur EMT est au maximum -1 qui n'est pas dans EMT
void pop(short x) {      
    if (x < 0 || x >= MP_SUP){  // invalid address
        throw_running_error("[pop]", 2);
    }
    else if (mp.SP <= 0) {  // stack underflow
        throw_running_error("[pop]", 3);
    }
    else {
        mp.SP--;
        mp.EMT[x] = mp.EMT[mp.SP];
        mp.PC++;
    }
}



void ipop(short x){
    if (mp.SP <= 1) // stack underflow
        throw_running_error("[ipop]", 3);

    else if (mp.EMT[mp.SP - 1] < 0 || mp.EMT[mp.SP - 1] >= MP_SUP) {    // invalid address
        throw_running_error("[ipop]", 2);
    }

    else {    
        short n = mp.EMT[mp.SP - 1];
        mp.EMT[n] = mp.EMT[mp.SP - 2];
        mp.SP -= 2;
        mp.PC++;
    }
}



void push(short x){
    if (mp.SP >= MP_SUP){   // stack overflow
        throw_running_error("[push]", 4);
    }
    else if (x < 0 || x >= MP_SUP) {    //  invalid address
        throw_running_error("[push]", 2);
    }
    else {
        mp.EMT[mp.SP] = mp.EMT[x];
        mp.SP++;
        mp.PC++;
    }
}



void ipush(short x){
    if (mp.EMT[mp.SP - 1] < 0 || mp.EMT[mp.SP - 1] >= MP_SUP){  // invalid address
        throw_running_error("[ipush]", 2);
    }
    else {   
        short n = mp.EMT[mp.SP - 1];  
        mp.EMT[mp.SP - 1] = mp.EMT[n];       //SP is not decremented as changes are direclty made to mp.EMT[SP - 1]
        mp.PC++;
    }
}



void push_(short i){
    if (mp.SP >= MP_SUP){   // stack overflow
        throw_running_error("[push# i]", 4);
    }
    else {
        mp.EMT[mp.SP] = i;
        mp.SP++;
        mp.PC++;
    }
}



void jmp(short adr){
    if (((mp.PC + adr) >= v->count) || ((mp.PC + adr) < 0)){ // faut initialiser ici une variable globale representant le vecteur d'instructions pour que ca marche
        throw_running_error("[jmp]", 6);
    }
    else if (adr == -1){    // infinite loop on the same instruction
        throw_running_error("[jmp]", 7);
    }
    else {  
        mp.PC += adr;
        mp.PC++;
    }
}



void jnz(short adr){          /*Faire attention au cas ou PC sort de l'intervalle permis*/
    if (mp.SP <= 0) {  // stack underflow
        throw_running_error("[jnz]", 3);
    }
    else {
        mp.SP--;
        if (mp.EMT[mp.SP]){
            if (((mp.PC + adr) >= v->count) || ((mp.PC + adr) < 0)){ // faut initialiser ici une variable globale representant le vecteur d'instructions pour que ca marche
                throw_running_error("[jnz]", 6);
            }
            else if (adr == -1){    // infinite loop on the same instruction
                throw_running_error("[jnz]", 7);
            }
            else {  
                mp.PC += adr;
                mp.PC++;
            }
        }
        else mp.PC++;
    }
}



void call(short adr) {
    if (mp.SP <0 || mp.SP >= MP_SUP){
        printf("Erreur [call]: SP en dehors de la Pile\n");
        exit(1);
    }
    if(mp.PC < SHRT_MIN || mp.PC > SHRT_MAX){
        printf("Erreur [call]: mp.PC n'est pas un short pour etre stocker sur la Pile");
        exit(1);
    }
    // Empiler PC sur la pile
    mp.EMT[mp.SP++] = mp.PC;            
    // Ajouter adr au registre PC
    mp.PC += adr;   /*on pourrait afficher un warning puisque PC sera bcp trop en dehors de la Pile mais sa fera trop de warnings si on effectue la tache bcp trop de fois*/

}



void ret(short){
    /*Demander au prof*/
}


// on pourra la remplacer par read_new juste en dessous ?
void read(short x){

    //Boucle pour s'assurer que l'utilisateur rentre bien un short

    while (1) {
        printf("Entrer une valeur (short) a placer dans la variable a l'adresse %d: \n", x);

        // Lire l'entree avec %hi
        // x n'a pas été testé, seg fault si supérieur à 5000 ?
        // en vrai pas besoin de tester le nombre d'entrée aussi osef on n'en lit qu'une
        short retour_scanf = scanf("%hi", &(mp.EMT[x]));

        if (retour_scanf == 1) {    //(verifier qu'on a une seule entree)
            // Verification des limites de short
            // ça ne marchera pas car l'entrée saisie par l'utilisateur a déja été mise dans la variable qui est de type short --> dépassemment de short déja commis
            // il ne sera pas détéctable car la valeur "boucle" dans la fourchette de nombres valides pour le type
            if (mp.EMT[x] >= SHRT_MIN && mp.EMT[x] <= SHRT_MAX) {
                break;
            } else {
                printf("Erreur : la valeur doit être comprise entre %d et %d.\n", SHRT_MIN, SHRT_MAX);
            }
        } else {
            printf("Erreur : veuillez entrer un nombre entier valide (short).\n");

            // Vider le buffer d'entree
            // il se videra petit a petit dans une seule boucle ou alors la premiere valeur valide sera choisie en input
            while (getchar() != '\n');
        }
    }

    mp.PC++;
}



void read_new(short x){

    if ( 0 <= x && x <= MP_SUP){
        long long int input = SHRT_MAX + 1;

        while ( ! ( SHRT_MIN <= input && input <= SHRT_MAX) ) {
            printf("Input (2 bytes max):\n");
            scanf("%lli", &input);
        }

        mp.EMT[x] = (short) input;

        mp.PC++;
    }

    else
        throw_running_error("[read]", 2);
}

 

void write(short x){
    if (0 <= x && x < MP_SUP) {
        printf("Value at address %d: %hd\n", x, mp.EMT[x]);
        mp.PC++;
    }
    else throw_running_error("[write]", 2); // invalid address
}


// a partir d'ici j'ai pas mis le nouveau système d'erreur en place il faudrait le faire


void op(short i) {  // ATTENTION TRAVAILLER LE CAS OU Y A QUE UN ELEMENT DANS LE STACKKK
    switch (i) {
        case 0: // Equality test
            if (mp.SP > MP_SUP){
                printf("Erreur op[0]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[0]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] == mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 1: // Test d'inegalite
            if (mp.SP > MP_SUP){
                printf("Erreur op[1]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[1]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] != mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 2: // Test >=
            if (mp.SP > MP_SUP){
                printf("Erreur op[2]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[2]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] >= mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 3: // Test <=
            if (mp.SP > MP_SUP){
                printf("Erreur op[3]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[3]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] <= mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 4: // Test >
            if (mp.SP > MP_SUP){
                printf("Erreur op[4]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[4]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] > mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 5: // Test <
            if (mp.SP > MP_SUP){
                printf("Erreur op[5]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[5]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] < mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 6: // | 
            if (mp.SP > MP_SUP){
                printf("Erreur op[6]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[6]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] |= mp.EMT[mp.SP];
            break;
        
        case 7: // ^ 
            if (mp.SP > MP_SUP){
                printf("Erreur op[7]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[7]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] ^= mp.EMT[mp.SP];
            break;

        case 8: // | 
            if (mp.SP > MP_SUP){
                printf("Erreur op[8]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[8]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] &= mp.EMT[mp.SP];
            break;

        case 9: // ~ 
            if (mp.SP > MP_SUP){
                printf("Erreur op[9]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[9]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] = ~(mp.EMT[mp.SP-1]);
            break;

        case 10: // Addition des deux valeurs au sommet 
            if (mp.SP > MP_SUP){
                printf("Erreur op[10]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[10]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] += mp.EMT[mp.SP];
            break;

        case 11: // Soustraction des deux valeurs au sommet 
            if (mp.SP > MP_SUP){
                printf("Erreur op[11]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[11]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] -= mp.EMT[mp.SP];
            break;

        case 12: // Multiplication des deux valeurs au sommet 
            if (mp.SP > MP_SUP){
                printf("Erreur op[12]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[12]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] *= mp.EMT[mp.SP];
            break;

        case 13: // Div. Entiere des deux valeurs au sommet 
            if (mp.SP > MP_SUP){
                printf("Erreur op[13]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[13]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] /= mp.EMT[mp.SP];
            break;

        case 14: // Modulo des deux valeurs au sommet 
            if (mp.SP > MP_SUP){
                printf("Erreur op[14]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 2) {
                printf("Erreur op[14]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.SP--;
            mp.EMT[mp.SP-1] %= mp.EMT[mp.SP];
            break;

        case 15: // Inverse la valeur au sommet 
            if (mp.SP > MP_SUP){
                printf("Erreur op[15]: SP depasse la limite sup. de EMT.\n");
                exit(1);
            }
            if (mp.SP < 1) {
                printf("Erreur op[15]: Pas assez d'elements sur la pile pour effectuer l'operation.\n");
                exit(1);
            }
            mp.EMT[mp.SP-1] = -mp.EMT[mp.SP-1];
            break;
        
        default:
            printf("Erreur op[i]: Operation inconnue (%d).\n", i);
            break;
    }

    mp.PC++;
}



void rnd(short x){      //ATTENTION. Faut etre sur que ce ne'est pas l'utilisateur qui rentre de valeur ici, sinon faut changer le code pour qu'il ne puisse uniquement entrer un truc de valide
    if (mp.SP < 0){ // Out of bounds (case almost impossible to happen)
        throw_running_error("[rnd]", 2);
    }
    else if (mp.SP >= MP_SUP){  // stack overflow
        throw_running_error("[rnd]", 4);
    }
    else {
        // Generating the random number between 0 and x-1
        short random_value = rand() % x;

        mp.EMT[mp.SP] = random_value;
        mp.SP++;
        mp.PC++;
    }
}



void dup(short x){
    if (mp.SP <= 0){    // Out of bounds (case almost impossible to happen)
        throw_running_error("[dup]", 2);
    }
    else if (mp.SP >= MP_SUP){  // stack overflow
        throw_running_error("[dup]", 4);
    }
    else {
        mp.EMT[mp.SP] = mp.EMT[mp.SP - 1];
        mp.SP++;
        mp.PC++;
    }
}

void halt(short x){

















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

    mp.EMT = init_mp();

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
        // the index in this array of the Instruction is its ADDRESS (line_no - 1, in the hexa file)
        // if the vector is full, the available size automatically increases (vector)
        Instruction_vector_append(&instructions, opcode, operand);
    }

    // running the instructions
    // warning: possibly an ininite loop if there is no halt in instructions
    // other program ending condition ?

    // while the instruction is not halt, and address in the right range
    while ( mp.PC < instructions.count && instructions.arr[mp.PC].opcode != 99 && ! mp.error){

        opcode = instructions.arr[mp.PC].opcode;
        operand = instructions.arr[mp.PC].operand;

        // PC is managed by the mp_functions functions individually
        // instruction execution
        mp_functions[opcode](operand);

    }

    // must call this function because of malloc call during init
    Instruction_vector_deconstruct(&instructions);
}