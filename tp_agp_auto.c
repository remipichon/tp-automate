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

int isAnnulable(NODE* root, int PorD) {
    
    if (PorD == _PP) {
        return root->fg->annulable;
    } else {
        return root->fd->annulable;
    }
    
   
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
                    (isAnnulable(root, _DP) && isAnnulable(root, _DP)))
                    ||
                    (root->type_node == NODE_AND &&
                    (isAnnulable(root, _DP) || isAnnulable(root, _DP))
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
            if (isAnnulable(node, PorD)) {
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

void setPosSuivante(NODE* node, ENS *posSuivante, int nbPos) {
    int i, j;

    if (node != NULL && feuille(node) == 0) {

        setPosSuivante(node->fg, posSuivante, nbPos);
        if (node->type_node != NODE_STAR) setPosSuivante(node->fd, posSuivante, nbPos); //parce qu'un star n'a pas de fils droit

        if (node->type_node == NODE_AND && node->fg->var != '#') {
            //pour toute les DP de fg
            for (i = 1; i <= nbPos; i++) {
                if (existeElem(node->fg->DP, i) == 1) {
                    //pour toute les PP de fg
                    for (j = 1; j <= nbPos; j++) {
                        if (existeElem(node->fd->PP, j)) {
                            //on ajoute la position j dans la possuivante de i
                            //printf("ajout de %d dans posSuivante(%d)\n", j, i);
                            ajoutElem(&posSuivante[i - 1], j);
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
                            ajoutElem(&posSuivante[i], j + 1);
                        }
                    }
                }
            }
        }
    }
}

int initRoot(NODE * root, int nbPos, int existeLettre[26], int *nbLettresExistantes) {
    if (root != NULL) {
        if (feuille(root)) {
            nbPos++;
            if (existeLettre[root->var - 'a'] == 0) {
                existeLettre[root->var - 'a'] = 1;
                (*nbLettresExistantes)++;
            }
        }
        nbPos = initRoot(root->fg, nbPos, existeLettre, nbLettresExistantes);
        nbPos = initRoot(root->fd, nbPos, existeLettre, nbLettresExistantes);
        //init des attributs
        root->PP = creerEnsemble();
    }
    return nbPos;
}

void remplirTableauLettres(int existeLettre[26], char *lettres) {
    int i, j = 0;
    for (i = 0; i < 26; i++) {
        if (existeLettre[i] == 1) {
            lettres[j++] = i + 'a';
        }
    }
}

int verifEtatNonMarque(int DTrans[][20], int nbEtat, int indiceEtatMarque) {
    int i;
    for (i = 0; i < nbEtat; i++) {
        if (DTrans[i][indiceEtatMarque] != 1)
            return i;
    }
    return -1;
}

int verifPosLettre(char *corresPosLettres, char lettre, int nbPos, int pos) {
    nbPos++; //pour eviter d'avoir un warnig
    if (corresPosLettres[pos - 1] == lettre)
        return 1;
    return 0;

}

int addEtat(ENS **etat, ENS ensTemp, int* nbEtat) {
    int i;
    for (i = 0; i<*nbEtat; i++) {
        if (egale(*etat[i], ensTemp))
            return i;
    }
    //sinon faut ajouter l'état
    etat[*nbEtat++] = ensTemp;
    return *nbEtat;
}

void marquerEtat(int DTrans[][20], int numEtat, int indiceEtatMarque) {
    DTrans[numEtat][indiceEtatMarque] = 1;

}

void printDTrans(int DTrans[][20], int nbCol, int nbLigne) {
    int i, j;
    for (i = 0; i < nbLigne; i++) {
        for (j = 0; j < nbCol + 1; j++) {
            printf("%d ", DTrans[i][j] );
        }
        printf("\n");

    }



}

void setDtrans(NODE* root, int DTrans[][20], ENS **etat, char *lettres, int nbLettre, ENS **posSuivante, int nbPos, char* corresPosLettres) {
//    printf("just call \n");
    int numEtat = 0;
    int noColone, pos, indiceEtat, nbEtat = 1;
    ENS ensTemp;
    //	1. Initialisation : le seul état de Dtrans non marqué est PremièrePos(racine)
    etat[numEtat] = root->PP;
//    printf("before while \n");
    //	2. Tant qu’il existe un état ETAT non marqué faire
    while ((numEtat = verifEtatNonMarque(DTrans, nbEtat, nbLettre + 1)) != -1) {
//        printf("etat (num etat): %d\n", numEtat);
        //	3. Pour chaque position (- la derniere)  faire
        for (noColone = 0; noColone < nbLettre; noColone++) {
//            printf("    les lettres : %d => %c \n", noColone, lettres[noColone]);

            ensTemp = creerEnsemble();

            for (pos = 1; pos <= nbPos; pos++) {
//                printf("        for pos pos : %d\n", pos);
//                printf("        ensETAT :");
                affichage(etat[numEtat]); //bon !
                if (existeElem(etat[numEtat], pos)) {
//                    printf("            val %d de l'ensemble ETAT no %d\n", pos, numEtat);
                    //pour chaque pos de ETAT
                    if (verifPosLettre(corresPosLettres, lettres[noColone], nbPos, pos) == 1) { //	la feuille de l’arbre à cette position contient la lettre LETTRE
                        //4. Soit ENS l’ensemble des PosSuivantes(POS), où POS est une position de ETAT telle que  
                        printf("posSuivante[%d]\n",pos-1);
                        affichage(posSuivante[pos-1]);
                        ensTemp = unionEns(ensTemp, posSuivante[pos-1]);
                        printf("                verif pos lettre\n");
                        printf("                ensTemp %d",&ensTemp);
                        affichage(ensTemp);
                    }
                }
            }

            //verifier si l'union ensTemp existe dans ENS* etat
            indiceEtat = addEtat(etat, ensTemp, &nbEtat); // si etat n'existe pas, on l'ajoute - getIndice
            DTrans[numEtat][noColone] = indiceEtat; //DTrans[numEtat][noColone] = indice    

        }
        marquerEtat(DTrans, numEtat, nbLettre + 1);

        printDTrans(DTrans, nbLettre + 1, nbEtat);
    }


}


/*-----------------------------------------------------------------*/
/*                 	POINT D'ENTREE DU TP                    	*/

/*-----------------------------------------------------------------*/

void tp(NODE * root) {
    printf("**********************   Debut :\n");
    int existeLettre[26]; //tableau d'existance lettres
    int i, nbLettresExistantes = 0;
    for (i = 0; i < 26; i++)
        existeLettre[i] = 0;
    int nbPos = initRoot(root, 0, existeLettre, &nbLettresExistantes); //init tableau lettres aussi
    char corresPosLettres[] = {'a', 'b', 'a', 'b', 'c'};



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

    printf("set pos suivante\n");
    ENS posSuivante[nbPos];
    //	ENS **posSuivante = (ENS**)malloc(sizeof(ENS*)*nbPos);
    for (i = 0; i < nbPos; i++) {
        posSuivante[i] = creerEnsemble();
    }
    setPosSuivante(root, posSuivante, nbPos);


    printf("affichage pos suviante\n");    
    for (i = 0; i < nbPos; i++) {
        printf("posSuivante(%d) : ", i + 1);
        affichage(posSuivante[i]);
    }
     


    int j;
    int DTrans[20][nbLettresExistantes + 1]; //matrice de transition
    for (i = 0; i < 20; i++)
        for (j = 0; j < nbLettresExistantes + 1; j++)
            DTrans[i][j] = 0;


    char lettres[nbLettresExistantes];
    remplirTableauLettres(existeLettre, lettres);

    /*
    printf("affichage des lettres existantes de l'expre reguliere\n");
    for (i = 0; i < 26; i++)
        printf("%d ", existeLettre[i]);
    printf("\n");

    printf("affichage des %d lettres de l'expre reguliere\n", nbLettresExistantes);
    for (i = 0; i < nbLettresExistantes; i++)
        printf("%c ", lettres[i]);
    printf("\n");
     */


    ENS * etat[20]; //tableau associant un etat à un ensemble
    printf("set Dtrans\n");
    setDtrans(root, DTrans, etat, lettres, nbLettresExistantes, posSuivante, nbPos, corresPosLettres);


}





