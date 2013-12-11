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
        printf("%c :\n", root->var);
        printf("pos : %d\n", root->position);
        printf("annulable : %d\n", root->annulable);
        printf("PP : ");
        affichage(root->PP);
        afficherDecoration(root->fd);
    }
}

int feuille(NODE *root) {
    if (root->fd == NULL && root->fg == NULL) return 1;
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

int isAnnulable(NODE* root) {
    return root->annulable;
}

void setAnnulable(NODE *root) {
    if (root != NULL) {
        setAnnulable(root->fg);
        setAnnulable(root->fd);
        if (feuille(root)) {
            root->annulable = 0; //une feuille n'est jamais annulable
        } else {
            if (root->type_node == NODE_STAR) {
                root->annulable = 1;
            } else if ((root->type_node == NODE_AND &&
                    (isAnnulable(root->fd) && isAnnulable(root->fd)))
                    ||
                    (root->type_node == NODE_AND &&
                    (isAnnulable(root->fd) || isAnnulable(root->fd))
                    )) {
                root->annulable = 1;
            } else {
                root->annulable = 0;
            }

        }
    }
}

void setPPNoeud_var(NODE* noeud, int var) {
    printf("creation PP pour feuille avec : %d\n", var);
    ENS ens = creerEnsemble();
    ajoutElem(&ens, var);
    noeud->PP = ens;
}

void setPPNoeud(NODE* noeud, ENS ens1, ENS ens2) {
    //pas propre ici !
    if (ens2 == NULL) ens2 = creerEnsemble();
    ENS ens = creerEnsemble();
    ens = unionEns(ens1, ens2);
    noeud->PP = ens;
}

void setPP(NODE* node) {
    if (node != NULL) {
        setPP(node->fg);
        setPP(node->fd);
        if (feuille(node)) {
            setPPNoeud_var(node, node->position);
        } else if (node->type_node == NODE_OR) {
            setPPNoeud(node, node->fg->PP, node->fd->PP);
        } else if (node->type_node == NODE_AND) {
            if (isAnnulable(node->fg)) {
                setPPNoeud(node, node->fg->PP, node->fd->PP);
            } else {
                setPPNoeud(node, node->fg->PP, NULL);
            }
        } else {
            setPPNoeud(node, node->fg->PP, NULL);
        }
    }

}

void initRoot(NODE* root) {
    if (root != NULL) {
        initRoot(root->fg);
        initRoot(root->fd);
        //init des attributs
        root->PP = creerEnsemble();
    }
}



/*-----------------------------------------------------------------*/
/*                     POINT D'ENTREE DU TP                        */

/*-----------------------------------------------------------------*/

void tp(NODE *root) {
    initRoot(root);
    //affichage regexp
    printf("regex : ");
    afficherER(root);
    printf("\n");


    printf("set Pos \n");
    setPos(root, 1);
    //afficherDecoration(root);
    printf("set Annulable\n");
    setAnnulable(root);
    //afficherDecoration(root);
    printf("set PP\n");
    setPP(root);
    afficherDecoration(root);




}






