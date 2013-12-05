/**
 * @file Aleatoire.c
 * @brief 
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 *
 */

#include "Aleatoire.h"

//-------------------------------------------------------------------------------------------------------------
//									  Déclaration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static char _LettreAleatoire(int nbRecursions);

//-------------------------------------------------------------------------------------------------------------
//											Gestion de l'aléatoire
//-------------------------------------------------------------------------------------------------------------

void Aleatoire_init(){
	srand(time(NULL));
}

/**
 * @fn static unsigned int Aleatoire_MinMax(int min, int max)
 * @brief Fonction qui retourne un nombre aléatoire 
 * compris entre un minimum et un maximum
 *
 * @param min Valeur minimale de la valeur à renvoyer
 * @param max Valeur maximale de la valeur à renvoyer
 *
 * @return Valeur aléatoire comprise entre min et max
 */
unsigned int Aleatoire_MinMax(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

/**
 * @fn static cpVect Aleatoire_Direction()
 * @brief Fonction qui retourne une direction aléatoire
 * comprise entre (0,-80) et (0,80)
 *
 * @return Direction aléatoire sous forme de vecteur
 */
cpVect Aleatoire_Direction(){
	int x = Aleatoire_MinMax(0,80);
	int dirGauche = Aleatoire_MinMax(0,1);

	if(dirGauche){
		return cpv(-x,0);
	}
	else {
		return cpv(x,0);
	}
}


/**
 * @var static Uint32 couleurs []
 * @brief Tableau de couleurs codées en RRGGBBAA
 *
 * Utilisé par @see Aleatoire_Couleur
 * voir http://fr.wikipedia.org/wiki/Liste_de_couleurs
 * pour plus de couleurs
 */ 
static Uint32 couleurs [] = { 
							   0xE67E30FF, /* Abricot */
							   0x74C0F1FF, /* Azur clair */
							   0xFFE436FF, /* Jaune Impérial */
							   0xD2CAECFF, /* Gris de lin */
							   0xC60800FF, /* Coquelicot */
							   0x26619CFF, /* Lapis-lazulli */
							   0x3A9D23FF  /* Vert gazon */
						    };

/**
 * @fn static Uint32 Aleatoire_Couleur()
 * @brief Fonction qui retourne couleur aléatoire
 * prise dans le tableau couleurs
 *
 * @see couleurs
 *
 * @return Couleur aléatoire sous la forme RRGGBBAA
 */
Uint32 Aleatoire_Couleur(){
	return couleurs[Aleatoire_MinMax(0,6)];
}

/**
 * @var static const int lettres []
 * @brief Tableau de probabilités d'apparition
 * d'un caractère le dictionnaire
 *
 * L'indice correspond à la position de la lettre 
 * dans l'alphabet. Les probabilités sont ajoutés.
 *
 */
#define TAILLE_TAB_LETTRES 26
#define RATIO_VOYELLES 43.41

static const float lettres [] = {
									 9.78,
									11.16,
									14.53,
									16.87,
									31.56,
									32.89,
									34.47,
									35.62,
									45.13,
									45.31,
									45.36,
									49.35,
									51.88,
									59.32,
									65.18,
									67.51,
									68.01,
									76.69,
									86.92,
									93.80,
									97.36,
									98.31,
									98.32,
									98.57,
									98.91,
								   100.00,
							    };

/**
 * @fn static char Aleatoire_Lettre()
 * @brief Fonction qui retourne une lettre majuscule
 * de l'alphabet aléatoirement en suivant les probabilités
 * d'apparition listés dans le tableau lettres
 *
 * @see lettres
 *
 * @return Couleur aléatoire sous la forme RRGGBBAA
 */
char Aleatoire_Lettre(){
	return _LettreAleatoire(0);
}

static char _LettreAleatoire(int nbRecursions){
	
	/**
	 * Nombre total de lettres générées
	 */
	static unsigned int lettresTotal = 0;

	/**
	 * Nombre de fois que chaque lettre de l'alphabet
     * a été générée
	 *
	 * nb_Lettres permet d'accéder aux statistique comme suit :
	 *   	nbLettre['A'] => nbLettre['Z']
	 */
	static unsigned int _lettresGeneres[26] = {0};
	static unsigned int* lettresGeneres = _lettresGeneres - 65;

	// Génération d'un nombre réél aléatoire entre 0 et 100
	const float proba = (rand()%10000)/100.0;

	// Cherche la lettre à renvoyer en fonction 
    // du nombre généré
	char lettre = 0;

	unsigned int i = 0, l = 0;
	while(lettre == 0 && i < TAILLE_TAB_LETTRES){	
		l = 65 + i;
		if(proba <= lettres[i++]){
			lettre = (char)l;	
			lettresGeneres[l]++;
		}
	}	

	// La proportion de la lettre trouvée par rapport aux nombre
	// de lettres générés est trop grande
	// Ou si le ratio voyelles/consonnes n'est pas correct (à 10% près)
    // ==> il faut générer un nouveau caractère
	float ratioVoyelles = (lettresGeneres['A'] + lettresGeneres['E'] 
							+ lettresGeneres['I'] +lettresGeneres['O'] + lettresGeneres['U']) / (float)lettresTotal;

	if(nbRecursions < 5 && lettresTotal > 10 && lettresGeneres[l] > 2){
		if(((lettresGeneres[l]/lettresTotal) > lettres[l]) 
	   		|| ratioVoyelles < RATIO_VOYELLES)
		{
			lettresGeneres[l]--;
			return _LettreAleatoire(++nbRecursions);	
		}
	}
	lettresTotal++;

	return lettre;
}

cpVect Aleatoire_Position(int rayon){

	static char p = 1;	
	p ^= 0x01;

	const unsigned int max = LARGUEUR_ECRAN - 2*OFFSET - rayon;

	if(p){ // 1 fois sur 2 dans la partie gauche du panier
		return cpv(Aleatoire_MinMax(OFFSET + rayon, max/2), -rayon);
	}
	else { // partie droite du panier
		return cpv(Aleatoire_MinMax(max/2, max), -rayon);
	}

}





