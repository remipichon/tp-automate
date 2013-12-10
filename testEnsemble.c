/* 
 * File:   testEnsemble.c
 * Author: Kiki
 *
 * Created on 9 décembre 2013, 17:13
 */

#include <stdio.h>
#include <stdlib.h>

#include "ensemble.h"

/*
 * 
 */




int main(int argc, char** argv) {
    int i,doublon;
    ENS ens1, ens2,ensUnion;
    printf("\n******************test creation****************** \n");
    ens1 = creerEnsemble();

    printf("\n******************test ajoutElem******************\n");
    for ( i = 10; i > 0; i--) {
        if (!ajoutElem(&ens1, i)) printf("%d n'est pas ajouté - ", i);
        else printf("%d est ajouté - ", i);      
    }
    
    printf("\n******************test affichage (0 à 10)****************** \n");
    affichage(ens1);
    printf("affichage OK\n");
       
    doublon = 5;
    if (!ajoutElem(&ens1, doublon)) printf("%d n'est pas ajouté (OK)", doublon);
    else printf("%d est ajouté (NOK)", doublon);
    printf("\n ajout OK \n");

    printf("\n******************test existeElem******************\n");
    for (i = 12; i > -2; i--) {
        if (!existeElem(ens1, i)) printf("%d n'est pas présent", i);
        else printf("existe %d - ", i);
    }
    printf("\n existe OK \n");

    printf("\n******************test union******************\n");    
    ens2 = creerEnsemble();
    printf("avec un ensemble vide\n");
    ensUnion = unionEns(ens1,ens2);
    affichage(ensUnion);
    printf("union avec ens vide OK\n");
    
    printf("union avec ensemble de 5 à 15 donc affiche de 0 à 15\n");
    //ajout des elements de 5 à 15
    for (i = 5; i < 15 ; i++) {
        ajoutElem(&ens2, i);        
    }
    ensUnion = unionEns(ens1,ens2);
    affichage(ensUnion);
    printf("union avec ens non vide OK\n");
    
    printf("\n******************test equal******************\n");
    printf(" non egal : %d",egale(ens1,ens2));
    
    //reinit des ensemble
    ens1 = creerEnsemble();
     for (i = 5; i < 15 ; i++) {
        ajoutElem(&ens1, i);        
    }
    printf(" egal : %d",egale(ens1,ens2));




    return (EXIT_SUCCESS);
}

