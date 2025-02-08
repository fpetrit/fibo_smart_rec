#ifndef RUNTIME_H
#define RUNTIME_H

typedef struct {
    int PC;
    int SP;
    short *EMT;
    unsigned char error;

} MP;

//short *init_mp(void);

void pop(short);
void ipop(short);
void push(short);
void ipush(short);
void push_(short);
void jmp(short);
void jnz(short);
void call(short);
void ret(short);
void read(short);
void write(short);
void op(short);
void rnd(short);
void dup(short);

void run(FILE * hexa);

#endif 