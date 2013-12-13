/* 
 * File:   ensemble.c
 * Author: Kiki
 *
 * Created on 9 décembre 2013, 17:59
 */

#include <stdio.h>
#include <stdlib.h>

#include "ensemble.h"

#define N 20

/**
 * Vérifie si l'index demandé est correcte
 * @param ensemble
 * @param index
 * @return val de l'index
 */
int getVal(ENS ensemble, int index) {
    if (0 <= index && index < ensemble->max)
        return ensemble->tab[index];
    printf("sortie de la capacité du tableau de ENS en get (indice demandé : %d, max : %d\n", index, ensemble->max);
    //exit(-1);
    return -1;
}

/**
 *  Vérifie si l'index demandé est correcte
 * @param ensemble
 * @param index
 * @param val
 * @return 1 si set ok
 */
int setVal(ENS ensemble, int index, int val) {
    if (0 <= index && index < ensemble->max) {
        ensemble->tab[index] = val;
        return 1;
    }
    printf("sortie de la capacité du tableau de ENS en set (indice demandé : %d, max : %d\n", index, ensemble->max);
    //exit(-1);
    return -1;
}

int max(int n, int m) {
    return (n > m) ? n : m;
}

int min(int n, int m) {
    return (n < m) ? n : m;
}

/**
 * initialise un tableau de taille case avec des 0
 * @param taille
 * @return 
 */
ENS creerEnsembleTaille(int taille) {
    int i;
    ENS newEns = (ENS) malloc(sizeof (ENS));
    newEns->max = taille;
    newEns->tab = (int*) malloc(sizeof (int)*newEns->max);
    for (i = 0; i < taille - 1; i++)
        setVal(newEns, i, 0);
    return newEns;
}

/**
 * méthode de l'interface, permet de prendre en compte une taille definie dure 
 * (#define N)
 * @return 
 */
ENS creerEnsemble() {
    return creerEnsembleTaille(N);
}


/**
 * cas tableau : position representée par un index+1 dans le tableau, valeur de 
 * la casse passe à 1 si ajout de l'element
 */
int ajoutElem(ENS* ensemble, int val) {
    if (getVal((*ensemble), val - 1) == 1) {
        printf("depassement capacité\n");
        return 0;
    }

    setVal((*ensemble), val - 1, 1); //(*ensembnle)->tab[val-1] = 1  
    return 1;
}

/**
 * cas tableau, juste le pointeur du tableau à free
 * @param ensemble
 * @return 
 */
int supprimerEnsemble(ENS ensemble) {
    free(ensemble->tab);
    return 1;
}

/**
 * parcours du tableau avec une mise en forme en ligne telle que :
 * {a,b,c}
 * @param ensemble
 */
void affichage(ENS ensemble) {
    int val, i;
    int firstPassed = 0; //pour un affichage elegant
    printf("{");
    for (i = 0; i < ensemble->max - 1; i++) {
        //printf("i : %d\n", i);       
        if ((val = getVal(ensemble, i)) == 1) {
            if (firstPassed) printf(", "); //pour un affichage elegant
            printf("%d", i + 1);
            firstPassed = 1; //pour un affichage elegant
        }
    }
    printf("}\n");

}


/**
 * cas tableau : la valeur de la case position (val) - 1 permet de savoir si l'element existe dans l'ensemble
 * @param ensemble
 * @param val
 * @return 
 */
int existeElem(ENS ensemble, int val) {
    return getVal(ensemble, val - 1);
}


/**
 * cas tableau : ajout des valeurs des deux tableaux en symetrie, la somme ne 
 * peut depasser 1 (absence de l'element = 0, sinon c'est 1)
 * @param ensemble1
 * @param ensemble2
 * @return 
 */
ENS unionEns(const ENS ensemble1, const ENS ensemble2) {
    ENS unionEns = creerEnsembleTaille(max(ensemble1->max, ensemble2->max));
    int i, val;

    for (i = 0; i < max(ensemble1->max, ensemble2->max); i++) {
        if (getVal(ensemble1, i) + getVal(ensemble2, i) > 0) val = 1;
        else val = 0;
        setVal(unionEns, i, val);
    }
    return unionEns;
}

/**
 * cas tablaau : comparaison symetrique des deux tableaux
 * @param ensemble1
 * @param ensemble2
 * @return 
 */
int egale(ENS ensemble1, ENS ensemble2) {
    int i;
    if (ensemble1->max != ensemble2->max) return 0;

    for (i = 0; i < ensemble1->max; i++)
        if (getVal(ensemble1, i) != getVal(ensemble2, i)) return 0;

    return 1;
}


int isEmpty(ENS ensemble){
    int i;
    for( i=1; i<=ensemble->max;i++){
        if( existeElem(ensemble,i))
            return 0;
    }
    return 1;
}
