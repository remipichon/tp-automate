/* 
 * File:   ensemble.c
 * Author: Remi Pichon
 *
 * Created on 9 décembre 2013, 17:59
 */


#include <stdio.h>
#include <stdlib.h>

#include "tree_agp.h"

#define _PP 1
#define _DP 2

/********************************************************/
/***************** fonction pour le debug **************/

/**
 * afficher une expression regulière à partir d'un arbre (parcours infixe)
 * @param root
 */
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

/**
 * affiche l'arbre décodé :
 *      positions
 *      annulable
 *      premières positions
 *      dernieres positions
 * @param root
 */
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


/*****************fin fonction pour le debug **************/
/********************************************************/

/**
 * determine si un noeud est une feuille ou non
 * @param root
 * @return 1 si feuille, 0 sinon
 */
int feuille(NODE *root) {
    if (root->fd == NULL && root->fg == NULL) return 1;
    return 0;
}

/**
 * determine si un noeud est annulable ou non
 * @param root
 * @param PorD
 * @return 1 si anulable, 0 sinon
 */
int isAnnulable(NODE* root, int PorD) {

    if (PorD == _PP) {
        return root->fg->annulable;
    } else {
        return root->fd->annulable;
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
        root->DP = creerEnsemble();
    }
    return nbPos;
}


/********************************************************/
/***************** decoration de l'abre ****************/
/* dans l'ordre d'appel, determination de :
 * positions
 * annulable
 * premiere position
 * derniere position 
 */

/**
 * determine pour chaque noeud sa position, valable uniquement pour les feuilles
 * @param root
 * @param cpt
 * @return 
 */
int setPos(NODE *root, int cpt) {
    if (root != NULL) {
        cpt = setPos(root->fg, cpt);
        cpt = setPos(root->fd, cpt);
        if (feuille(root)) root->position = cpt++;
    }
    return cpt;
}

/**
 * determine le caractère annulable de chaque noeud (parcours postfixe)
 * @param root
 */
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

/**** premieres et dernieres positions  *****/
/*
 * Dans la mesure ou l'algo est très proche pour les deux traitements, une seule
 * fonction setPDP(NODE, int PorD) permet d'effectuer premieres et dernieres 
 * positions. Le flag PorD (premiereOrDerniere) permet, à l'appel, de determiner
 * quel traitement est effectué. Les fonctions setPDPNoeud_var et setPDPNoeud
 * sont appelées par setPDP avec le flag et ce sont elles qui determinent si il
 * faut renseingner l'ensemble PP ou DP du noeud (entre autre).
 */

/**
 * cas feuille, on ajoute un position dans l'ensemble (PP ou DP)
 * @param noeud
 * @param var : position à ajouter
 * @param PorD
 */
void setPDPNoeud_var(NODE* noeud, int var, int PorD) {
    ENS ens = creerEnsemble();
    ajoutElem(&ens, var);
    if (PorD == _PP) // si PorD = PremierePosition, on traite l'ensemble PP (premiere position)
        noeud->PP = ens;
    else
        noeud->DP = ens;
}

/**
 * fait l'union des ensembles du fils gauche et du fils droit pour le stocker 
 * dans l'ensemble du noeud. PorD permet de determiner si on traite PP ou DP. 
 * prend également en charge le cas ou FD est null recquis par NODE_STAR qui n'a
 * pas de fils droit.
 * @param noeud
 * @param FG noeud fils gauche
 * @param FD noeuf fils droit
 * @param PorD premiereOrDerniere
 */
void setPDPNoeud(NODE* noeud, NODE* FG, NODE* FD, int PorD) {
    //pas propre ici !
    if (FD == NULL) FD = (NODE*) malloc(sizeof (NODE)); //call by NODE_STAR
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

    if (ens2 == NULL) ens2 = creerEnsemble(); //cas NODE_STAR

    ens = unionEns(ens1, ens2);

    if (PorD == _PP)
        noeud->PP = ens;
    else
        noeud->DP = ens;

}

/**
 * determine l'ensemble des premieres/dernieres positions pour chaque noeud via 
 * un parcours postfixe
 * @param node
 * @param PorD : PremiereOrDerniere : _PP (premiere position) ou bien _DP 
 * (derniere position)
 */
void setPDP(NODE* node, int PorD) {
    if (node != NULL) {
        setPDP(node->fg, PorD);
        setPDP(node->fd, PorD);
        if (feuille(node)) { //une feuille ne contient que sa propre postion PP DP
            setPDPNoeud_var(node, node->position, PorD);
        } else if (node->type_node == NODE_OR) {
            setPDPNoeud(node, node->fg, node->fd, PorD);
        } else if (node->type_node == NODE_AND) {
            if (isAnnulable(node, PorD)) {
                setPDPNoeud(node, node->fg, node->fd, PorD);
            } else {
                //cas différent entre PP et DP
                if (PorD == _PP)
                    setPDPNoeud(node, node->fg, NULL, PorD); //union avec fils gauche
                else
                    setPDPNoeud(node, node->fd, NULL, PorD); //union avec fils droit
            }
        } else //cas NODE_STAR
            setPDPNoeud(node, node->fg, NULL, PorD);
    }
}


/********************************************************/
/****** resolution de la table des transitions***********/

/**
 * determine le tableaux des positions suivantes, parcours postfixe
 * @param node
 * @param posSuivante : tableau des ensembles des positions suivantes pour
 * chaque position 
 * @param nbPos : nombre position, taille de posSuivante
 */
void setPosSuivante(NODE* node, ENS *posSuivante, int nbPos) {
    int i, j;
    if (node != NULL && feuille(node) == 0) {

        //reccursivité
        setPosSuivante(node->fg, posSuivante, nbPos);
        if (node->type_node != NODE_STAR)
            setPosSuivante(node->fd, posSuivante, nbPos); //parce qu'un star n'a pas de fils droit

        if (node->type_node == NODE_AND && node->fg->var != '#') {
            //pour toute les DernieresPositions de filsGauche
            for (i = 1; i <= nbPos; i++) {
                if (existeElem(node->fg->DP, i) == 1) {
                    //pour toute les PpremieresPositions de filsGauche
                    for (j = 1; j <= nbPos; j++) {
                        if (existeElem(node->fd->PP, j)) {
                            //on ajoute la position j dans la possuivante de i
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
                            ajoutElem(&posSuivante[i], j + 1);
                        }
                    }
                }
            }
        }
    }
}

/*****   fonction pour algo de determination de la table des transtions  ****/
/*
 * les méthodes/fonctions ci dessous sont utiliser par DTrans qui a besoin de 
 * nombreux tableaux
 * 
 * int DTrans[20][4]    matrice de la table des transtions, contient les indices 
 * des différents états présents dans 
 * ENS *etat            tableau de chaque états (stockés dans des ENSemble) 
 * char *lettres        tableau des lettres présentes dans le regex (sans doublons)
 * int nbLettre         nb de lettre dans char *lettres
 * ENS *posSuivante     tableau des positions suivantes, posSuivante[i] = ENSsemble
 * int nbPos            nb de positions dans posSuivante (nb de positions tout court)
 * char* corresPosLettres
 *                      correspondance entre les positions et les lettres dans les
 *              feuilles de l'arbre (taille = nbPos)
 */

/**
 * recupere toutes les lettres du regex dans un tableau temporaire existeLettre 
 * (qui contient donc des 'trous'
 * @param existeLettre : tableau de 26 cases, une case par lettre de l'alphabet
 * @param lettres
 */
void remplirTableauLettres(int existeLettre[26], char *lettres) {
    int i, j = 0;
    for (i = 0; i < 26; i++) {
        if (existeLettre[i] == 1) {
            lettres[j++] = i + 'a';
        }
    }
}

/**
 * passe en revue tous les etats (lignes) de DTrans pour vérifier s'il reste des
 * états à traiter, si c'est le cas, renvoit l'indice de l'état dans DTrans
 * @param DTrans
 * @param nbEtat
 * @param indiceEtatMarque  : postion de le colonne des etats marqués dans DTrans 
 *      (situé à nbLettre +1 (juste après les collones de lettre -1 pour avoir un
 *      indice de tableau)
 * @return 
 */
int verifEtatNonMarque(int DTrans[20][4], int nbEtat, int indiceEtatMarque) {
    int i;
    // printf("verif etat non marque : nb etat :%d \n",nbEtat);
    for (i = 0; i < nbEtat; i++) {
        if (DTrans[i][indiceEtatMarque] != 1)
            return i;
    }
    return -1;
}

/**
 * verifie si la position (feuille de l'arbre) comprend la lettre
 * @param corresPosLettres
 * @param lettre        lettre des collones de DTrans
 * @param pos           positions 
 * @return 
 */
int verifPosLettre(char *corresPosLettres, char lettre, int pos) {
    if (corresPosLettres[pos - 1] == lettre)
        return 1;
    return 0;

}

/**
 * ajouter dans les états l'état temporaire que si pas déja présent et renvoit
 * l'indice du nouveau etat dans le tableau des états, si déja présent, renvoit
 * juste l'indice de l'état dans le tableau des états
 * @param etat          tableau des etats
 * @param ensTemp       ensemble à eventuellement ajouter
 * @param nbEtat        nb case de tableau des etats
 * @return 
 */
int addEtat(ENS *etat, ENS ensTemp, int* nbEtat) {
    int i;
    for (i = 0; i<*nbEtat; i++) {
        if (egale(etat[i], ensTemp)) //etat déja existant
            return i + 1;
    }
    //sinon faut ajouter l'état
    etat[(*nbEtat)++] = ensTemp; //le ++ est prioritaire sur le * !!!!
    return *nbEtat;
}

/**
 * passe à état à terminé
 * @param DTrans
 * @param numEtat
 * @param indiceEtatMarque
 */
void marquerEtat(int DTrans[20][4], int numEtat, int indiceEtatMarque) {
    DTrans[numEtat][indiceEtatMarque] = 1;
}

/**
 * debug : affiche la table des transitions
 * @param DTrans
 * @param nbLigne
 * @param nbCol
 */
void printDTrans(int DTrans[20][4], int nbLigne, int nbCol) {
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 4; j++)
            printf("%d ", DTrans[i][j]);
        printf("\n");

    }
}

/**
 * debug : affiche le tableau des états 
 * @param etat
 * @param nbEtat
 */
void printEtat(ENS *etat, int nbEtat) {
    int i;
    printf("%d Etat : ", nbEtat);
    for (i = 0; i < nbEtat; i++) {
        affichage(etat[i]);
    }
    printf("\n");
}

/**
 * determine la table des transitions 
 * (pour les arguments, voir commentaires plus
 * haut)
 */
void setDtrans(NODE* root, int DTrans[20][4], ENS *etat, char *lettres, int nbLettre, ENS *posSuivante, int nbPos, char* corresPosLettres) {
    int numEtat = 0;
    int cptLigneParcours = 0;
    int noColone, pos, indiceEtat, nbEtat = 1;
    ENS ensTemp;
    //	1. Initialisation : le seul état de Dtrans non marqué est PremièrePos(racine)
    etat[numEtat] = root->PP;
    //	2. Tant qu’il existe un état ETAT non marqué faire
    while ((numEtat = verifEtatNonMarque(DTrans, nbEtat, nbLettre)) != -1) {
        //	3. Pour chaque position (- la derniere)  faire
        for (noColone = 0; noColone < nbLettre; noColone++) {
            ensTemp = creerEnsemble();
            for (pos = 1; pos <= nbPos; pos++)
                if (existeElem(etat[numEtat], pos))
                    //pour chaque pos de ETAT
                    if (verifPosLettre(corresPosLettres, lettres[noColone], pos)) //	la feuille de l’arbre à cette position contient la lettre LETTRE
                        //4. Soit ENS l’ensemble des PosSuivantes(POS), où POS est une position de ETAT telle que  
                        ensTemp = unionEns(ensTemp, posSuivante[pos - 1]);


            //verifier si l'union ensTemp existe dans ENS* etat
            //l'ensemble vide ne fait pas parti des etats
            if (!isEmpty(ensTemp)) {
                indiceEtat = addEtat(etat, ensTemp, &nbEtat); // si etat n'existe pas, on l'ajoute - getIndice
                DTrans[numEtat][noColone] = indiceEtat; //DTrans[numEtat][noColone] = indice    
            }

        }
        marquerEtat(DTrans, numEtat, nbLettre);
        cptLigneParcours++;
    }
}


/********************************************************/
/****** ecriture du fichier c ***********/


int setAcceptation(int *tableauAcceptation, ENS *etat, int nbEtat, int nbPos){
    int nbAcceptation = 0;
    int i;
    for(i = 0; i < nbEtat ; i++ ){
        affichage(etat[i]);
        if( existeElem(etat[i], nbPos) ){ //si diese present dans l'état, c'est un etat d'acceptation
            tableauAcceptation[nbAcceptation++] = i;
            
        }
    }
    
    return nbAcceptation;
    
    
}



void ecrireFichier(int Dtrans[20][4], int* tableauAcceptation, int nbEetat, int nbEtatAffectation) {
    int i, j;
    FILE *f;
    //on copie la "tÃªte" du fichier
    system("cat automate_head.c > monautomate.c");
    //puis on l'ouvre en Ã©criture avec le curseur de flux Ã  la fin
    f = fopen("monautomate.c", "a");

    //on ecrit les Ã©tats d'acceptation
    for (i = 0; i < nbEtatAffectation; i++) {
        //if (tableauAcceptation[i] == 1)
            fprintf(f, "\nstatus_etat[%d]=ACCEPTATION;\n", tableauAcceptation[i]);
    }
    fprintf(f, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            "}",
            "/*----------------------------------------------------------------------*/",
            "void construire_transition()",
            "{",
            " int i,j;",
            "",
            " /* Par defaut, toutes les transitions sont interdites */",
            " for(i=0;i<MAX_ETAT;i++)",
            " for(j=0;j<26;j++)",
            " transition[i][j]=-1;",
            "",
            " /* On ajoute Ã  la main les transitions */",
            " /* Note : a=0, b=1, ..., z=25 */",
            "");
    //puis les Transitions
    for (i = 0; i < nbEetat; i++) {
        for (j = 0; j < 3; j++) {
            if (Dtrans[i][j] != 0) {
                fprintf(f, " /* transition %d lettre %c %d */\n", i, j + 97, Dtrans[i][j]);
                fprintf(f, " transition[%d][%d]=%d;\n", i, j, Dtrans[i][j]-1);  //car je n'ai pas d'état 0 !!
            }
        }
    }
    fclose(f); //on le referme une fois l'Ã©criture dynamique finie
    //et on fini d'Ã©crire la partie statique
    system("cat automate_tail.c >> monautomate.c");
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
    char corresPosLettres[] = {'a', 'b', 'a', 'b', 'c','#'}; //TODO via de l'algo)



    printf("regex : ");
    afficherER(root);
    printf("\n");


    printf("set Pos..\n");
    setPos(root, 1);
    printf("set Annulable..\n");
    setAnnulable(root);
    printf("set PP..\n");
    setPDP(root, _PP);
    printf("set DP ..\n");
    setPDP(root, _DP);
    printf("affichage de l'arbre décoré (via un parcours postfixe), c'est Noel ! \n");
    afficherDecoration(root);

    printf("set pos suivante..\n");
    ENS posSuivante[nbPos]; //TODO en dynamique
    for (i = 0; i < nbPos; i++) {
        posSuivante[i] = creerEnsemble();
    }
    setPosSuivante(root, posSuivante, nbPos);


    printf("affichage pos suviante\n");
    for (i = 0; i < nbPos; i++) {
        printf("posSuivante(%d) : ", i + 1);
        affichage(posSuivante[i]);
    }
    printf("\n");




    int j;
    //    int DTrans[20][nbLettresExistantes + 1]; //matrice de transition
    int DTrans[20][4]; //matrice de transition  //TODO en dynamique
    for (i = 0; i < 20; i++)
        for (j = 0; j < 4; j++)
            DTrans[i][j] = 0;

    //printDTrans(DTrans, 20, nbLettresExistantes + 1); //putain de probleme d'affichage


    char lettres[nbLettresExistantes];
    remplirTableauLettres(existeLettre, lettres);

    int nbEtat = 4;
    
    ENS etat[20]; //tableau associant un etat à un ensemble //TODO overcapacity
    printf("set Dtrans..\n");
    setDtrans(root, DTrans, etat, lettres, nbLettresExistantes, posSuivante, nbPos, corresPosLettres);

    printf("\n\n\nTable de transition\n");
    printDTrans(DTrans, nbEtat, 4); //TODO en dynamique, n'afficher que les etats qu'il faut (nbEtat)
    
    printf("set tableauAcception...\n");
    int tableauAcceptation[nbEtat]; //au plus, nbEtat d'acceptation
    
    int nbAcceptation = setAcceptation(tableauAcceptation, etat, nbEtat, nbPos);
    
    printf("Etat d'acceptation : \n"); 
    for( i=0; i < nbAcceptation; i++ )
        affichage(etat[tableauAcceptation[i]]);
    
    printf("write output file...\n");
    ecrireFichier(DTrans, tableauAcceptation, nbEtat+1, nbAcceptation);
    
    
    printf("compilation de l'automate avec gcc monautomate.c -o auto \n");
  

}





