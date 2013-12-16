}
/*----------------------------------------------------------------------*/
char lire(){
  return( (char)getchar());
}
/*----------------------------------------------------------------------*/
int code_lettre(char l)
{
  return((int)(l-'a'));
}
/*----------------------------------------------------------------------*/
int automate()
{
  char lu; /* caractere lu */
  int e=0; /* etat courant */

  printf("Entrer un mot suivit d'un point (.):\n");

  lu=lire();
 
 while(lu !='.'){
#ifdef TRACE 
  printf("état %d, lettre lue %c, prochaine état %d\n",e,lu,transition[e][code_lettre(lu)]);
#endif
   e=transition[e][code_lettre(lu)];
   if(e==-1) return(ECHEC);
   
   lu=lire();
  }

 return status_etat[e];
}

/*----------------------------------------------------------------------*/
/*                                MAIN                                  */
/*----------------------------------------------------------------------*/
int main()
{
  char lu;

  construire_status_etat();
  construire_transition();

  if(automate()==ECHEC) printf("Non reconnu !\n"); else printf("Reconnu !\n");


}