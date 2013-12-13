




/* gestion tableau */
///*
typedef struct ensembleTableau {
    int* tab;
    int elemCourant;
    int max;
} *ENS;
//*/

/* gestion structure */
/*
typedef struct ensembleStruct {
    int val;
    struct ensembleStruct* suivant;
} ensStruct;

typedef ensStruct* ENS;
//*/

/**
 * @return un ensemble initialisé 
 */
ENS creerEnsemble();

/**
 * Ajouter un element (une position) dans un ensemble si l'element n'est pas déja présent
 * @param ensemble
 * @param val : position à ajouter 
 * @return boolean, 1 : ajout Ok, 0 : pas d'ajout car doublon
 */
int ajoutElem(ENS* ensemble, int val);

/**
 * Supprimer correcter (les mallocs) un ensemble
 * @param ensemble
 * @return 
 */
int supprimerEnsemble(ENS ensemble);

//void retirerElem(ENS* ensemble, int val);

/**
 * Vérifier si un element existe dans l'ensemble
 * @param ensemble
 * @param val
 * @return 1 si trouvé, 0 sinon
 */
int existeElem(ENS ensemble, int val);

/**
 * \brief : retourne l'union de deux ensemble
 * \args : deux ensembles non modifiables
 * \precondition : ensemble valide
 * \return : l'union des deux ensembles
 * 
 */
ENS unionEns(const ENS ensemble1, const ENS ensemble2);

/**
 * Vérifie si deux ensembles contiennent les mêmes elements (positions)
 * @param ensemble1
 * @param ensemble2
 * @return 1 si egaux, 0 sinon
 */
int egale(ENS ensemble1, ENS ensemble2);


int isEmpty(ENS ensemble);


/**
 * pour du debug uniquement
 */
void affichage(ENS);