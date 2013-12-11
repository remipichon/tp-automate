




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


ENS creerEnsemble();

int ajoutElem(ENS* ensemble, int val);

int supprimerEnsemble(ENS ensemble);

//void retirerElem(ENS* ensemble, int val);

int existeElem(ENS ensemble, int val);

/**
 * \brief : retourne l'union de deux ensemble
 * \args : deux ensembles non modifiables
 * \precondition : ensemble valide
 * \return : l'union des deux ensembles
 * 
 */
ENS unionEns(const ENS ensemble1, const ENS ensemble2);

ENS interEns(const ENS ensemble1, const ENS ensemble2);

int egale(ENS ensemble1, ENS ensemble2);




/**
 * pour du debug uniquement
 */
void affichage(ENS);