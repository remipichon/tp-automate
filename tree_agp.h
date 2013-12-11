/*-----------------------------------

Node information definition
(syntaxe evaluation)

   Stephane Ubeda

-------------------------------------*/

#define NODE_VAR    1
#define NODE_OR     2
#define NODE_AND    3
#define NODE_STAR   4

#define SIZE 25

#define VRAI 1
#define FAUX 0

typedef struct node {
    int type_node; /* type de noeud          */
    char var; /* variable               */
    struct node *fg; /* fils gauche            */
    struct node *fd; /* fils droit             */
    
    int position;   /* position pos */
    int annulable;  /* annulable oui :1, non : 0*/

} NODE;


NODE *root;




