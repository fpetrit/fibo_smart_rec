#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

void run(FILE * hexa);

struct MP{                  /*MP = Machine a Pile*/     
    int PC;                 /*On initialisera plus tard les valeurs de mp.PC et mp.SP dans le main a 0. Bon c est fait normalement*/
    int SP;
    short *EMT;        /*EMT = Espace Memoire de Travail. Je ne dois pas oublier de rajouter le malloc plus tard!!*/

}mp = {0, 0, NULL};     /*initialiser EMT[] dans le main avec la fonction initialiser_MP ecrite juste en dessous*/

typedef struct MP MP;

short *initialiser_MP(void){
    short *p = malloc(5000 * sizeof(short));
    if(!p){
        printf("Pas reussi a allouer pour espace memoire\n");
        return 0;
    }
    return p;
}

/*La je commence a coder les opcode*/

void pop(short x){                  /*x est l'adresse*/
    if (x < 0 || x >= 5000){
        printf("Erreur [pop]: Adresse x hors limites\n");
        return;
    }
    if (mp.SP <= 0) {
        printf("Erreur [pop]: Pile vide\n");
        return;
    }
    mp.SP --;
    mp.EMT[x] = mp.SP;
}

void ipop(void){
    if (mp.SP <= 1) {
        printf("Erreur [ipop]: Pile vide\n");
        return;

    if (mp.EMT[(mp.SP)-1]< 0 || mp.EMT[(mp.SP)-1] >= 5000) {
        printf("Erreur [ipop]: Adresse x hors limites\n");
        return;
    }

    }
    int n = mp.EMT[mp.SP -1];       /*n = valeur du sommet de la pile*/
    mp.EMT[n] = mp.EMT[mp.SP - 2];
    mp.SP -= 2;
}

void push(short x){
    if (mp.SP >= 5000){         /*En gros le cas ou SP devient egal a 5000 marche mais si on push avec SP deja egal a 5000 ca ne marche pas*/
        printf("Erreur [push]: Limite sup. Pile atteinte\n");
        return;
    }
    mp.EMT[mp.SP] = mp.EMT[x];
    mp.SP ++;
}

void ipush(void){
    if (mp.EMT[mp.SP - 1] < 0 || mp.EMT[mp.SP - 1] >= 5000){
        printf("Erreur [ipush]: valeur contenue dans le sommet de la Pile n'est pas compris dans l'intervalle de l'espace memoire de travail\n");
        return;
    }
    int n = mp.EMT[mp.SP -1];  
    mp.EMT[mp.SP -1] = mp.EMT[n];       /*SP n est pas decremente comme la place de SP-1 est directement prise*/
}

void push_(short i){             /*La je veux remplacer le _ par # mais je n'arrive pas, stp fred help*/
    if (mp.SP >= 5000){         /*En gros le cas ou SP devient egal a 5000 marche mais si on push avec SP deja egal a 5000 ca ne marche pas*/
        printf("Erreur [push_]: Limite sup. Pile atteinte\n");
        return;
    }
    mp.EMT[mp.SP] = i;
    mp.SP ++;
}

void jmp(short adr){
    mp.PC += adr;
    if (mp.PC >= 5000 || mp.PC <= -1){
        printf("Erreur [jmp]: PC en dehors de la Pile\n");       /*C'est plutot un warning puisque ptet on reajoute a PC un nombre qui le replace dans la memoire*/
        return;
    }
}
void jnz(short adr){          /*Faire attention au cas ou PC sort de l'intervalle permis*/
    mp.SP --;
    if (mp.EMT[mp.SP]){
        mp.PC = adr + mp.PC;
    }
}

void call(short adr) {
    if (mp.SP <0 || mp.SP >= 5000){
        printf("Erreur [call]: SP en dehors de la Pile\n");
        return;
    }
    if(mp.PC < (-32768) || mp.PC > 32767){
        printf("Erreur [call]: mp.PC n'est pas un short pour etre stocker sur la Pile");
        return;
    }
    // Empiler PC sur la pile
    mp.EMT[mp.SP++] = mp.PC;            
    // Ajouter adr au registre PC
    mp.PC += adr;                       /*on pourrait afficher un warning puisque PC sera bcp trop en dehors de la Pile mais sa fera trop de warnings si on effectue la tache bcp trop de fois*/

}

void ret(void){
    /*Demander au prof*/
}

void read(int x){

    //Boucle pour s'assurer que l'utilisateur rentre bien un short

    while (1) {
        printf("Entrer une valeur (short) a placer dans la variable a l'adresse %d: \n", x);

        // Lire l'entrée avec %hi
        int retour_scanf = scanf("%hi", &(mp.EMT[x]));

        if (retour_scanf == 1) {    //(verifier qu'on a une seule entree)
            // Vérification des limites de short
            if (mp.EMT[x] >= SHRT_MIN && mp.EMT[x] <= SHRT_MAX) {
                break;
            } else {
                printf("Erreur : la valeur doit être comprise entre %d et %d.\n", SHRT_MIN, SHRT_MAX);
            }
        } else {
            printf("Erreur : veuillez entrer un nombre entier valide (short).\n");

            // Vider le buffer d'entrée
            while (getchar() != '\n');
        }
    }
}

void write(int x){
    if (x<0 || x>=5000){
        printf("Pas une adresse valide\n");
    }
    else{
        printf("Variable a l'adresse %d: %hi\n", x, mp.EMT[x]);
    }
}
void op(int i) {
    switch (i) {
        case 0: // Test d'egalite
            if (mp.SP > 5000){
                printf("Erreur op[0]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[0]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] == mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 1: // Test d'inegalite
            if (mp.SP > 5000){
                printf("Erreur op[1]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[1]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] != mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 2: // Test >=
            if (mp.SP > 5000){
                printf("Erreur op[2]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[2]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] >= mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 3: // Test <=
            if (mp.SP > 5000){
                printf("Erreur op[3]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[3]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] <= mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 4: // Test >
            if (mp.SP > 5000){
                printf("Erreur op[4]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[4]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] > mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 5: // Test <
            if (mp.SP > 5000){
                printf("Erreur op[5]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[5]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            if (mp.EMT[mp.SP - 1] < mp.EMT[mp.SP]) {
                mp.EMT[mp.SP - 1] = 1;
            } else {
                mp.EMT[mp.SP - 1] = 0;
            }
            break;

        case 6: // | 
            if (mp.SP > 5000){
                printf("Erreur op[6]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[6]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] |= mp.EMT[mp.SP];
            break;
        
        case 7: // ^ 
            if (mp.SP > 5000){
                printf("Erreur op[7]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[7]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] ^= mp.EMT[mp.SP];
            break;

        case 8: // | 
            if (mp.SP > 5000){
                printf("Erreur op[8]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[8]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] &= mp.EMT[mp.SP];
            break;

        case 9: // ~ 
            if (mp.SP > 5000){
                printf("Erreur op[9]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[9]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] = ~(mp.EMT[mp.SP-1]);
            break;

        case 10: // Addition des deux valeurs au sommet 
            if (mp.SP > 5000){
                printf("Erreur op[10]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[10]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] += mp.EMT[mp.SP];
            break;

        case 11: // Soustraction des deux valeurs au sommet 
            if (mp.SP > 5000){
                printf("Erreur op[11]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[11]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] -= mp.EMT[mp.SP];
            break;

        case 12: // Multiplication des deux valeurs au sommet 
            if (mp.SP > 5000){
                printf("Erreur op[12]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[12]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] *= mp.EMT[mp.SP];
            break;

        case 13: // Div. Entiere des deux valeurs au sommet 
            if (mp.SP > 5000){
                printf("Erreur op[13]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[13]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] /= mp.EMT[mp.SP];
            break;

        case 14: // Modulo des deux valeurs au sommet 
            if (mp.SP > 5000){
                printf("Erreur op[14]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 2) {
                printf("Erreur op[14]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.SP--;
            mp.EMT[mp.SP-1] %= mp.EMT[mp.SP];
            break;

        case 15: // Inverse la valeur au sommet 
            if (mp.SP > 5000){
                printf("Erreur op[15]: SP depasse la limite sup. de EMT.\n");
                return;
            }
            if (mp.SP < 1) {
                printf("Erreur op[15]: Pas assez d'éléments sur la pile pour effectuer l'opération.\n");
                return;
            }
            mp.EMT[mp.SP-1] = -mp.EMT[mp.SP-1];
            break;
        
        default:
            printf("Erreur op[i]: Opération inconnue (%d).\n", i);
            break;
    }
}



void rnd(short x){      //ATTENTION. Faut etre sur que ce ne'est pas l'utilisateur qui rentre de valeur ici, sinon faut changer le code pour qu'il ne puisse uniquement entrer un truc de valide
    if (mp.SP < 0){
        printf("Erreur [rnd]: Stack Pointeur en dessous de la borne inf. \n");  //Presque imp que ca se passe mais bon
        return;
    }
    else if (mp.SP >= 5000){
        printf("Erreur [rnd]: Stack Pointeur au dessus de la borne sup. \n");
        return;
    }
    // Génération du nombre aléatoire entre 0 et x-1
    short random_value = rand() % x;

    // Empilement du nombre sur la pile
    mp.EMT[mp.SP] = random_value;
    mp.SP++; // Incrémentation du pointeur de pile
}

void dup(void){
    if (mp.SP <= 0){
        printf("Erreur [rnd]: Stack Pointeur en dessous de la borne inf. \n");  //Presque imp que ca se passe mais bon sauf cas ou SP est a 0 (rien dans EMT)
        return;
    }
    else if (mp.SP >= 5000){    //warning pour le cas ou SP pointe sur le dernier element ccar SP pointe apres ca sur 5000 qui n est pas dans EMT
        printf("Erreur [rnd]: Stack Pointeur au dessus de la borne sup. \n");
        return;
    }
    mp.EMT[mp.SP] = mp.EMT[mp.SP - 1];
    mp.SP++;
}

void halt(void){        // doit etre mis dans le main pour faire effet
    printf("Simulation terminée. Fin du programme.\n"); 
    exit(0); // Arrête l'exécution du programme
}

/*ptet c mieux dinitilaiser dans la struct de MP PC et SP en tant que shorts*/
int main(void){
    mp.EMT = initialiser_MP();  /*Allouer la memoire dynamique pour l espace memoire de stockage*/
}

#endif