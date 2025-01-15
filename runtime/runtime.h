#ifndef RUNTIME_H
#define RUNTIME_H

//void run(FILE*);

typedef struct MP{                  /*MP = Machine a Pile*/     
    int PC;                 /*On initialisera plus tard les valeurs de mp.PC et mp.SP dans le main a 0. Bon c est fait normalement*/
    int SP;
    short *EMT;        /*EMT = Espace Memoire de Travail. Je ne dois pas oublier de rajouter le malloc plus tard!!*/

}MP;

extern MP mp;

short *initialiser_MP(void);
void pop(short);
void ipop(void);
void push(short);
void ipush(void);
void push_(short);
void jmp(short);
void jnz(short);
void call(short);
void ret(void);
void read(short);
void write(short);
void op(short);
void rnd(short);
void dup(void);
void halt(void);

#endif 