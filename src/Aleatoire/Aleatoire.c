/**
 * @file Aleatoire.c
 * @brief Gestion de l'al�atoire pour diff�rents �l�ments
 * @author J�r�my.B
 * @version 1.0
 * @date 6 d�cembre 2013
 *
 * Gestion de l'al�atoire pour un entier, une direction, une lettre, une position et une couleur
 *
 */

#include "Aleatoire.h"

//-------------------------------------------------------------------------------------------------------------
//									  D�claration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static char _LettreAleatoire(int nbRecursions);

//-------------------------------------------------------------------------------------------------------------
//											Gestion de l'al�atoire
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void Aleatoire_init()
 * @brief Fonction qui initialise le g�n�rateur de nombres al�atoires
 */
void Aleatoire_init(){
	srand(time(NULL));
}

/**
 * @fn static unsigned int Aleatoire_MinMax(int min, int max)
 * @brief Fonction qui retourne un nombre al�atoire 
 * compris entre un minimum et un maximum
 *
 * @param min Valeur minimale de la valeur � renvoyer
 * @param max Valeur maximale de la valeur � renvoyer
 *
 * @return Valeur al�atoire comprise entre min et max
 */
unsigned int Aleatoire_MinMax(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

/**
 * @fn static cpVect Aleatoire_Direction()
 * @brief Fonction qui retourne une direction al�atoire
 * comprise entre (0,-80) et (0,80)
 *
 * @return Direction al�atoire sous forme de vecteur
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
 * @brief Tableau de couleurs cod�es en RRGGBBAA
 *
 * Utilis� par @see Aleatoire_Couleur
 * voir http://fr.wikipedia.org/wiki/Liste_de_couleurs
 * pour plus de couleurs
 */ 
static Uint32 couleurs [] = { 
							   0xE67E30FF, /* Abricot */
							   0x74C0F1FF, /* Azur clair */
							   0xFFE436FF, /* Jaune Imp�rial */
							   0xD2CAECFF, /* Gris de lin */
							   0xC60800FF, /* Coquelicot */
							   0x26619CFF, /* Lapis-lazulli */
							   0x3A9D23FF  /* Vert gazon */
						    };

/**
 * @fn static Uint32 Aleatoire_Couleur()
 * @brief Fonction qui retourne couleur al�atoire
 * prise dans le tableau couleurs
 *
 * @see couleurs
 *
 * @return Couleur al�atoire sous la forme RRGGBBAA
 */
Uint32 Aleatoire_Couleur(){
	return couleurs[Aleatoire_MinMax(0,6)];
}

/**
 * @var static const int lettres []
 * @brief Tableau de probabilit�s d'apparition
 * d'un caract�re le dictionnaire
 *
 * L'indice correspond � la position de la lettre 
 * dans l'alphabet. Les probabilit�s sont ajout�s.
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
 *
 * Fonction qui retourne une lettre majuscule
 * de l'alphabet al�atoirement en suivant les probabilit�s
 * d'apparition list�s dans le tableau lettres
 *
 * @see lettres
 *
 * @return Couleur al�atoire sous la forme RRGGBBAA
 */
char Aleatoire_Lettre(){
	return _LettreAleatoire(0);
}

/**
 * @fn static char _LettreAleatoire(int nbRecursions)
 * @see Aleatoire_Lettre
 */
static char _LettreAleatoire(int nbRecursions){
	
	/**
	 * Nombre total de lettres g�n�r�es
	 */
	static unsigned int lettresTotal = 0;

	/**
	 * Nombre de fois que chaque lettre de l'alphabet
     * a �t� g�n�r�e
	 *
	 * nb_Lettres permet d'acc�der aux statistique comme suit :
	 *   	nbLettre['A'] => nbLettre['Z']
	 */
	static unsigned int _lettresGeneres[26] = {0};
	static unsigned int* lettresGeneres = _lettresGeneres - 65;

	// G�n�ration d'un nombre r��l al�atoire entre 0 et 100
	const float proba = (rand()%10000)/100.0;

	// Cherche la lettre � renvoyer en fonction 
    // du nombre g�n�r�
	char lettre = 0;

	unsigned int i = 0, l = 0;
	while(lettre == 0 && i < TAILLE_TAB_LETTRES){	
		l = 65 + i;
		if(proba <= lettres[i++]){
			lettre = (char)l;	
			lettresGeneres[l]++;
		}
	}	

	// La proportion de la lettre trouv�e par rapport aux nombre
	// de lettres g�n�r�s est trop grande
	// Ou si le ratio voyelles/consonnes n'est pas correct (� 10% pr�s)
    // ==> il faut g�n�rer un nouveau caract�re
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

/**
 * @fn cpVect Aleatoire_Position(int rayon)
 * @brief Retourne une position al�atoire en haut du panier
 *
 * @param rayon rayon de la balle (pour la placer au dessus de la fen�tre)
 * @return cpVect Coordonn�es du centre de la balle
 */
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





