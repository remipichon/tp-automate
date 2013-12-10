%{
/*--------------------------------------------------------------

             TP AGP : contruction de l'arbre
             synatxique d'une expression reguliere

                       Stephane Ubeda
                         24/10/2000

-----------------------------------------------------------------*/

#include <stdio.h>
#include <malloc.h>  
#include <stdlib.h> 
#include <memory.h> 
#include "tree_agp.h"
#include "node.h"

 %}   

%token   VARIABLE
%token   STAR
%token   OR
%token   AND
%token   SHARP

//associativite des operateurs
%left OR
%left AND
%left STAR

//type de yylval
 %union {
    NODE *NODE_TYPE;
   char *c;
 };

%type  <c> VARIABLE
%type  <NODE_TYPE> STAR
%type  <NODE_TYPE> OR
%type  <NODE_TYPE> AND
%type  <c> SHARP

%type <NODE_TYPE> expr
%type <NODE_TYPE> ok

%start ok_ext        
 
%%

ok_ext : ok
       {root=$1;YYACCEPT;}

ok : expr AND SHARP
        {$$=creerNODE(NODE_AND,".",$1,creerNODE(NODE_VAR,$3,0,0));}

expr :  VARIABLE             
           { $$=creerNODE(NODE_VAR,$1,0,0);}  
     | '(' expr ')' 
           {$$=$2;}
     | expr OR expr
          {$$=creerNODE(NODE_OR,"+",$1,$3);}
     | expr AND expr
         {$$=creerNODE(NODE_AND,".",$1,$3);}
     | expr STAR
             {$$=creerNODE(NODE_STAR,"*",$1,0);;}

%%
/*--------------------------------------------------------         
                  FONCTIONS LEX/YACC
--------------------------------------------------------*/
#include "lex.yy.c"

extern void tp(NODE *);

int yywrap(){
return 1;
}



int yyerror()
{
printf("\n erreur !\n");
 exit(1);
}

/*--------------------------------------------------------         
                FONCTIONS CREATION D'ARBRES
--------------------------------------------------------*/
NODE *creerNODE(int type,char v[],NODE *fg,NODE *fd)
{
  NODE *x;

  x=(NODE *)malloc(sizeof(NODE));

  x->type_node=type;
  x->var=v[0];

  x->fd=fd;
  x->fg=fg;

  return (x);
}
/*------------------------------------------------------         
                        MAIN

               Call to function yyparse()
               Call to student function tp
               with root as parameters.
--------------------------------------------------------*/
int main(){

yyparse();

   tp(root); 
}


