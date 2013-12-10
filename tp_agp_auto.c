#include <stdio.h>

#include "tree_agp.h"

void afficherER(NODE *root) {

    if (root != NULL) {
        if (root->fg != NULL && root->fd != NULL)
            printf("(");
        afficherER(root->fg);
        printf("%c", root->var);
        afficherER(root->fd);
        if (root->fg != NULL && root->fd != NULL)
            printf(")");
    }
}

void afficherDecoration(NODE *root) {

    if (root != NULL) {
        afficherDecoration(root->fg);
        printf("%c", root->var);
        printf("\n pos : %d \n", root->position);
        afficherDecoration(root->fd);
    }
}

int feuille(NODE *root) {
    if (root->fd == NULL && root->fg == NULL  ) return 1;
    return 0;
}

int setPos(NODE *root, int cpt) {
    if (root != NULL) {
        cpt = setPos(root->fg, cpt);

        cpt = setPos(root->fd, cpt);
        
        if (feuille(root)) root->position = cpt++;     
    }
    return cpt;
}

int setAnnulable(NODE *root){
    
}


/*-----------------------------------------------------------------*/
/*                     POINT D'ENTREE DU TP                        */

/*-----------------------------------------------------------------*/

void tp(NODE *root) {
    //affichage regexp
    //    printf("regex : ");
    //    afficherER(root);
    //    printf("\n");

    setPos(root, 1);    
    afficherDecoration(root);
    
    


}






