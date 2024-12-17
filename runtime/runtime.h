#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdio.h>

void run(FILE * hexa);

struct MP{                  /*MP = Machine a Pile*/     
    int PC;                 /*On initialisera plus tard les valeurs de mp.PC et mp.SP dans le main a 0. Bon c est fait normalement*/
    int SP;
    short EMT[5000];        /*EMT = Espace Memoire de Travail. Je ne dois pas oublier de rajouter le malloc plus tard!!*/

}mp;     
typedef struct MP MP;


/*La je commence a coder les opcode*/

 void pop(int x){       /*x est l'adresse*/
    mp.SP --;
    mp.EMT[x] = mp.SP;
 }

void ipop(void){
    int n = mp.EMT[mp.SP -1];       /*n = valeur du sommet de la pile*/
    mp.EMT[n] = mp.EMT[mp.SP - 2];
    mp.SP -= 2;
}

void push(int x){
    mp.EMT[mp.SP] = mp.EMT[x];
    mp.SP ++;
}

void ipush(void){
    int n = mp.EMT[mp.SP -1];  
    mp.EMT[mp.SP -1] = mp.EMT[n];       /*SP n est pas decremente comme la place de SP-1 est directement prise*/
}

void push_(i){             /*La je veux remplacer le _ par # mais je n'arrive pas, stp fred help*/
    mp.EMT[mp.SP] = i;
    mp.SP ++;
}

int main(void){
    mp.PC = 0;
    mp.SP = 0;
}

#endif