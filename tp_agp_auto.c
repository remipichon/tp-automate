#include <stdio.h>

#include "tree_agp.h"

#define _PP 1
#define _DP 2

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
        printf("DP : ");
        affichage(root->DP);
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

void setPDPNoeud_var(NODE* noeud, int var, int PorD) {
    ENS ens = creerEnsemble();
    ajoutElem(&ens, var);
    if (PorD == _PP)
        noeud->PP = ens;
    else
        noeud->DP = ens;
}

void setPDPNoeud(NODE* noeud, NODE* FG, NODE* FD, int PorD) {
    //pas propre ici !
    if (FD == NULL) FD = (NODE*) malloc(sizeof(NODE));
    ENS ens = creerEnsemble();
    ENS ens1 = creerEnsemble();
    ENS ens2 = creerEnsemble();

    if (PorD == _PP) {
        ens1 = FG->PP;
        ens2 = FD->PP;
    } else {
        ens1 = FG->DP;
        ens2 = FD->DP;
    }
    
    if (ens2 == NULL) ens2 = creerEnsemble();

    ens = unionEns(ens1, ens2);
    
     if (PorD == _PP) {
        noeud->PP = ens;
    } else {
        noeud->DP = ens;
    }
}

/**
 * PorD : premiere ou derniere
 */
void setPDP(NODE* node, int PorD) {
    if (node != NULL) {
        setPDP(node->fg, PorD);
        setPDP(node->fd, PorD);
        if (feuille(node)) {
            setPDPNoeud_var(node, node->position, PorD);
        } else if (node->type_node == NODE_OR) {
            setPDPNoeud(node, node->fg, node->fd, PorD);
        } else if (node->type_node == NODE_AND) {
            if (isAnnulable(node->fg)) {
                setPDPNoeud(node, node->fg, node->fd, PorD);
            } else {
                if( PorD == _PP )
                    setPDPNoeud(node, node->fg, NULL, PorD);
                else
                    setPDPNoeud(node, node->fd, NULL, PorD);
            }
        } else {
            setPDPNoeud(node, node->fg, NULL, PorD);
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
    setPDP(root, _PP);
    setPDP(root,_DP);
    afficherDecoration(root);




}






