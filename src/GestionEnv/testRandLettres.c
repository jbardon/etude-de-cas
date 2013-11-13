
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAILLE_TAB_LETTRES 26
#define RATIO_VOYELLES 43,41

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

static char _randLettre(){
	
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
printf("proba: %2.2f\n", proba);

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

	float ratioVoyelles = 0.0;
	ratioVoyelles += lettresGeneres['A'] + lettresGeneres['E'];
	ratioVoyelles += lettresGeneres['I'] +lettresGeneres['O'] + lettresGeneres['U'];
	ratioVoyelles /= lettresTotal;

printf("%c %d %2.2f\n",lettre,lettresTotal, ratioVoyelles);
	if(lettresTotal > 20){
		if(((lettresGeneres[lettre]/lettresTotal) > lettres[lettre]) 
	   		|| ratioVoyelles < RATIO_VOYELLES)
		{
			lettresGeneres[lettre]--;
			return _randLettre();	
		}
	}
	lettresTotal++;

	return lettre;
}

int main(void){

	srand(time(NULL));
	
	for(int i = 0; i < 25; i++){
		printf("%c ", _randLettre());
	}

	return 0;
}
