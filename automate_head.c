/*

Codage d'un automate travaillant sur l'alphabet a..z,
ayant au maximum MAX_ETAT états,
et lisant ces entrées dans stdin.

Pour le mode verbose :
 gcc automate.c -DTRACE -o automate      

Stephane Ubeda  - TC - 21/12/2000

 */

#include <stdio.h>

#define MAX_ETAT 25
#define ACCEPTATION 0
#define ECHEC       1


/* Tableau du status des etats (ACCEPTATION ou ECHEC) */
int status_etat[MAX_ETAT];

/* Tableau des transition MAX_ETAT x 26 */
int transition[MAX_ETAT][26];

/*----------------------------------------------------------------------*/
void construire_status_etat()
{
  int i;

  /* Par defaut, ils sont en status ECHEC */
  for(i=0;i<MAX_ETAT;i++) status_etat[i]=ECHEC;

  /* On ajoute à la main les etats ACCEPTATION */
