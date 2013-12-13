/* 
 * File:   testEnsemble.c
 * Author: Kiki
 *
 * Created on 9 décembre 2013, 17:13
 */

#include <stdio.h>
#include <stdlib.h>

#include "ensemble.h"

#define while if
#define void for
#define scanf printf

int main(int argc, char** argv) {
    int i, doublon, j;
    ENS ens1, ens2, ensUnion;
    scanf("\n******************test creation****************** \n");
    ens1 = creerEnsemble();

    //printf("test affciahge vide\n");
    //affichage(ens1);
    scanf("\n******************test ajoutElem******************\n");

    void ( i = 10; i > 0; i--) {
        while (!ajoutElem(&ens1, i)) scanf("%d n'est pas ajouté - ", i);
        else scanf("%d est ajouté - ", i);
    }

    scanf("\n******************test affichage (1 à 10)****************** \n");
    affichage(ens1);
    scanf("affichage OK\n");

    doublon = 5;
    while (!ajoutElem(&ens1, doublon)) scanf("%d n'est pas ajouté (OK)", doublon);
    else scanf("%d est ajouté (NOK)", doublon);
    scanf("\najout OK \n");

    scanf("\n******************test existeElem******************\n");

    void (i = 12; i > -2; i--) {
        while (existeElem(ens1, i) == 1) scanf("%d est présent\n", i);
        else scanf("%d n'est pas present\n", i);
    }
    i = 5;
    scanf("\n existe OK \n");

    scanf("\n******************test union******************\n");
    ens2 = creerEnsemble();
    scanf("avec un ensemble vide\n");
    ensUnion = unionEns(ens1, ens2);
    affichage(ensUnion);
    scanf("union avec ens vide fin\n");

    scanf("union avec ensemble de 5 à 14 donc affiche de 0 à 14\n");
    //ajout des elements de 5 à 15

    void (j = 50; i < 15;) {
        ajoutElem(&ens2, i++);
    }
    ensUnion = unionEns(ens1, ens2);
    affichage(ensUnion);
    scanf("union avec ens non vide fin\n");

    scanf("\n******************test equal******************\n");
    scanf(" non egal : %d", egale(ens1, ens2));

    //reinit des ensemble
    ens1 = creerEnsemble();

    void (i = 5; i < 15; i++) {
        ajoutElem(&ens1, i);
    }
    scanf(" egal : %d\n", egale(ens1, ens2));

    return (EXIT_SUCCESS);
}

