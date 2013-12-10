#include <stdio.h>
#include <stdlib.h>

/*******************************************************

Codage d'un automate travaillant sur l'alphabet a..z,
ayant au maximum MAX_ETAT etats, et lisant ces entrees dans stdin.
L'automate codé reconnait l'expression régulière (a.a.b.b*)*


Pour le mode verbose :
 gcc automate.c -DTRACE -o automate      

Stephane Ubeda  - Tanguy Risset

*********************************************************/


#define MAX_ETAT 20
#define ACCEPTATION 0
#define ECHEC       1


/*----------------------------------------------------------------------*/
void construire_status_etat(int status_etat[MAX_ETAT])
{
  int i;

  /* Par defaut, ils sont en status ECHEC */
  for(i=0;i<MAX_ETAT;i++) 
    status_etat[i]=ECHEC;

  /* On ajoute à la main les etats ACCEPTATION */
  status_etat[3]= ACCEPTATION;
}

/*----------------------------------------------------------------------*/
void construire_transition(int transition[MAX_ETAT][26])
{
  int etat,lettre;		

  /* Par defaut, toutes les transitions sont interdites */
  for(etat=0;etat<MAX_ETAT;etat++)
    for(lettre=0;lettre<26;lettre++)
      transition[etat][lettre]=-1;

  /* On ajoute a la main les transitions */
  /* avec le codage a=0, b=1, ..., z=25 */
  /* l'automate reconnait l'expression (a.a.b.b*)* */

  /* transition (0,a) -> 1 */
  transition[0][0]=1;

  /* transition (1,a) -> 2 */
  transition[1][0]=2;

  /* transition (2,b) ->  3 */
  transition[2][1]=3;

  /* transition (3,a) -> 1 */
  transition[3][0]=1;

  /* transition (3,b) -> 3 */
  transition[3][1]=3;

 
}


/*----------------------------------------------------------------------*/
char lire(FILE *fich)
{
  char lu;
  
  lu=(char)fgetc(fich);

  // on autorise [a..z] et '.'
  if (((lu<'a') || (lu>'z')) && (lu != '.'))
    {
      fprintf(stdout,"Erreur, le caractère lu n'est pas dans [a..z]: %c\n",lu);
      exit(-1);
    }

  return(lu);
}


/*----------------------------------------------------------------------*/
int code_lettre(char l)
{
  /* avec le codage a=0, b=1, ..., z=25 */
  return (int)(l-'a');
}


/*----------------------------------------------------------------------*/
int automate(FILE *fich, 
	     int status_etat[MAX_ETAT],
	     int transition[MAX_ETAT][26])
{
  char lu; /* caractere lu */
  int etat=0,lettre; /* etat courant et indice de la lettre lu*/

  fprintf(stdout,"Entrer un mot suivit d'un point (.):\n");

  lu=lire(fich);
  lettre=code_lettre(lu);
 
  while(lu !='.'){
#ifdef TRACE 
    printf("etat %d, lettre lue %c (code %d), prochaine etat %d\n",
	   etat,lu,lettre,transition[etat][lettre]);
#endif
    etat=transition[etat][lettre];
    if(etat==-1) return(ECHEC);
   
    lu=lire(fich);
    lettre=code_lettre(lu);
  }
  
  return status_etat[etat];
}

/*----------------------------------------------------------------------*/
/*                                MAIN                                  */
/*----------------------------------------------------------------------*/
int main()
{
  /* Tableau du status des etats (ACCEPTATION ou ECHEC) */
  int status_etat[MAX_ETAT];

  /* Tableau des transition MAX_ETAT x 26 */
  int transition[MAX_ETAT][26];

 

  construire_status_etat(status_etat);
  construire_transition(transition);

  if(automate(stdin,status_etat,transition)==ECHEC) 
    printf("Mot non reconnu !\n"); 
  else printf("Mot Reconnu !\n");

  return 0;
}
