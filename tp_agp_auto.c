#include <stdio.h>
#include <stdlib.h>

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
    if (root->fd == NULL && root->fg == NULL
            && root->type_node != NODE_STAR //KIKI pas sur de son utilité !
            ) return 1;
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
    if (FD == NULL) FD = (NODE*) malloc(sizeof (NODE));
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

    if (PorD == _PP)
        noeud->PP = ens;
    else
        noeud->DP = ens;

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
                if (PorD == _PP)
                    setPDPNoeud(node, node->fg, NULL, PorD);
                else
                    setPDPNoeud(node, node->fd, NULL, PorD);
            }
        } else
            setPDPNoeud(node, node->fg, NULL, PorD);
    }
}

void setPosSuivante(NODE* node, ENS** posSuivante, int nbPos) {
    int i, j;

    if (node != NULL && feuille(node) == 0) {

        setPosSuivante(node->fg, posSuivante, nbPos);
        if (node->type_node != NODE_STAR) setPosSuivante(node->fd, posSuivante, nbPos); //parce qu'un star n'a pas de fils droit

        if (node->type_node == NODE_AND) {
            //pour toute les DP de fg
            for (i = 1; i <= nbPos; i++) {
                if (existeElem(node->fg->DP, i) == 1) {
                    //pour toute les PP de fg
                    for (j = 1; j <= nbPos; j++) {
                        if (existeElem(node->fd->PP, j)) {
                            //on ajoute la position j dans la possuivante de i
                            //printf("ajout de %d dans posSuivante(%d)\n", j, i);
                            ajoutElem(&(posSuivante[i - 1]), j);
                        }
                    }
                }
            }
        } else if (node->type_node == NODE_STAR) {
            //pour toute les positions de PremierePose(n)
            for (i = 0; i <= nbPos; i++) {
                if (existeElem(node->DP, i + 1)) { //i est une position de dernierepos(n)
                    //pour toutes les possitions PP(n)
                    for (j = 0; j <= nbPos; j++) {
                        if (existeElem(node->PP, j + 1)) {
                            //printf("ajout de %d dans posSuivante(%d)\n", j + 1, i + 1);
                            ajoutElem(&(posSuivante[i]), j + 1);
                        }
                    }
                }
            }
        }
    }
}

int initRoot(NODE * root, int nbPos) {
    if (root != NULL) {
        if (feuille(root)) nbPos++;
        nbPos = initRoot(root->fg, nbPos);
        nbPos = initRoot(root->fd, nbPos);
        //init des attributs
        root->PP = creerEnsemble();
    }
    return nbPos;
}

void setDtrans(NODE* root, int *DTrans, ENS **etat, int nbEtat) {
    int numEtat = 0;
    int numLettre = 0;
    //    1. Initialisation : le seul état de Dtrans non marqué est PremièrePos(racine)
    etat[numEtat] = root->PP;
    //    2. Tant qu’il existe un état ETAT non marqué faire
    while () {
        //    3. Pour chaque position (- la derniere)  faire
        for each(position - 1 = > pos) {
            //    4. Soit ENS l’ensemble des PosSuivantes(POS), où POS est une position de ETAT telle que
            //    la feuille de l’arbre à cette position contient la lettre LETTRE
            
            //    5. Si ENS n’est pas vide et ENS nouvel état faire
            //    6. Ajouter ENS non marqué aux états
            //    7. Ajouter Dtrans[ETAT,LETTRE]=ENS
            //    8. Fin si
            //    9. Si ENS non vide et ENS existe déjà faire
            //    10. Ajouter Dtrans[ETAT,LETTRE]=ENS
            //    11. Fin si
            //    12. Fin pour
        }
        //    13. Marquer ETAT
    }
    //    14. Fin tant que

}
/*-----------------------------------------------------------------*/
/*                     POINT D'ENTREE DU TP                        */

/*-----------------------------------------------------------------*/

void tp(NODE * root) {
    printf("**********************   Debut :\n");
    int nbPos = initRoot(root, 0);
    printf("nb pos : %d\n", nbPos);

    printf("regex : ");
    afficherER(root);
    printf("\n");


    printf("set Pos \n");
    setPos(root, 1);
    printf("set Annulable\n");
    setAnnulable(root);
    printf("set PP\n");
    setPDP(root, _PP);
    setPDP(root, _DP);
    printf("affichage de l'arbre décoré, c'est Noel ! \n");
    afficherDecoration(root);

    int i;
    printf("set pos suivante\n");
    ENS * posSuivante[nbPos];
    //    ENS **posSuivante = (ENS**)malloc(sizeof(ENS*)*nbPos);
    for (i = 0; i < nbPos; i++) {
        posSuivante[i] = creerEnsemble();
    }
    setPosSuivante(root, &posSuivante, nbPos);


    printf("affichage pos suviante\n");
    for (i = 0; i < nbPos; i++) {
        printf("posSuivante(%d) : ", i + 1);
        affichage(posSuivante[i]);
    }

    printf("set Dtrans\n");
    int DTrans[nbPos][20]; //matrice de transition
    ENS * etat[20]; //tableau associant un etat à un ensemble

    setDtrans(root, DTrans, etat, nbPos);




}






