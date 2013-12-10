/* 
 * File:   ensemble.c
 * Author: Kiki
 *
 * Created on 9 décembre 2013, 17:59
 */

#include <stdio.h>
#include <stdlib.h>

#include "ensemble.h"

#define N 11

ENS creerEnsemble() {
    int i;
    ENS newEns = (ENS) malloc(sizeof (ENS));
    newEns->max = N;
    newEns->tab = (int*) malloc(sizeof (int)*newEns->max);
    for (i = 0; i < N-1; i++)
        newEns->tab[i] = 0;
    return newEns;
}

int ajoutElem(ENS* ensemble, int val) {
    (*ensemble)->tab[val + 1] = 1;

    return 1;
}

int supprimerEnsemble(ENS ensemble) {
    free(ensemble->tab);
    return 1;
}

void affichage(ENS ensemble) {
    printf("__");
    int val, i;
    for (i = 0; i < ensemble->max; i++)
        if (val = ensemble->tab[i] == 1)
            printf("%d - ", i+1);
    printf("__\n"); 

}


int existeElem(ENS ensemble, int val){
    return NULL;
}


ENS unionEns(const ENS ensemble1, const ENS ensemble2){
    return NULL;
}

ENS interEns(const ENS ensemble1, const ENS ensemble2){
    return NULL;
}

int egale(ENS ensemble1, ENS ensemble2){
    return 1;
}