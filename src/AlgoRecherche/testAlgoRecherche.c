#include <stdio.h>
#include <stdlib.h>
#include <config.h>

#include "AlgoRecherche.h"

int main()
{	
	printf("----> Test de la HashTable <---- \n");
	//création de la table de hashage pour ranger le dico
		GHashTable* dico = chargerDico(RES("/AlgoRecherche/dico.txt"));

//**** AFFICHAGE DE LA TABLE ****//
/*		//Variable de l'iterateur pour la table de hashage
		GHashTableIter iter;
		char *value;
		int *key;
		g_hash_table_iter_init(&iter,DicoHash); //iterator pour afficher le hashtable
		while (g_hash_table_iter_next (&iter, (gpointer)&key, (gpointer)&value))
		{
			//printf("key %d ---> %s\n", *key, value);
		}

	printf("----> Test de la Version 1 <---- \n");
	char*subtring=version1("dxabaissaiz",DicoHash);
*/

	return EXIT_SUCCESS;
}



