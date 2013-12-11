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


int getVal(ENS ensemble,int index){
	if( 0 <= index && index < ensemble->max )
			return ensemble->tab[index];	
	return -1;
}

int setVal(ENS ensemble, int index, int val){
	if( 0 <= index && index < ensemble->max ){
			ensemble->tab[index] = val;
			return 1;
	}		
	return -1;
}	

int max(int n, int m){
	return (n>m)? n: m;
}	

int min(int n, int m){
	return (n<m)? n: m;
}	



ENS creerEnsembleTaille(int taille) {
    int i;
    ENS newEns = (ENS) malloc(sizeof (ENS));
    newEns->max = taille;
    newEns->tab = (int*) malloc(sizeof (int)*newEns->max);
    for (i = 0; i < taille-1; i++)
        setVal(newEns,i,0);
    return newEns;
}

ENS creerEnsemble(){
	return creerEnsembleTaille(N);
}	

int ajoutElem(ENS* ensemble, int val) {
		if( getVal((*ensemble), val - 1) == 1 ) return 0;
		
    setVal( (*ensemble), val - 1,  1);    //(*ensembnle)->tab[val-1] = 1  
    return 1;
}

int supprimerEnsemble(ENS ensemble) {
    free(ensemble->tab);
    return 1;
}

void affichage(ENS ensemble) {
		printf("{");
    int val, i;
    for (i = 0; i < ensemble->max-1; i++)
        if ( (val = getVal(ensemble,i) )== 1)
            printf("%d,", i+1);
    printf("}\n"); 

}


int existeElem(ENS ensemble, int val){
    return getVal(ensemble, val-1);
}


ENS unionEns(const ENS ensemble1, const ENS ensemble2){
		ENS unionEns = creerEnsembleTaille(max(ensemble1->max,ensemble2->max));
		int i,val;
		
		for(i=0; i< max(ensemble1->max,ensemble2->max); i++){
				if( getVal(ensemble1,i) + getVal(ensemble2,i) > 0) val = 1;
				else  val = 0;
				setVal(unionEns,i, val);
		}
    return unionEns;
}

ENS interEns(const ENS ensemble1, const ENS ensemble2){
    return NULL;
}

int egale(ENS ensemble1, ENS ensemble2){
		int i;
		if(ensemble1->max != ensemble2->max) return 0;
		
		for(i=0; i< ensemble1->max; i++)
				if( getVal(ensemble1,i) != getVal(ensemble2,i) ) return 0;
		
		
	  return 1;
}
