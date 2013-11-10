#include <stdio.h>
#include <stdlib.h>
#include <config.h>

#include "AlgoRecherche.h"

int main()
{	

	//création de la table de hashage pour ranger le dico
	printf("\n----> Test chargerDico <---- \n");
	GHashTable* dico = chargerDico(RES("/AlgoRecherche/dico.txt"));

	//**** AFFICHAGE DE LA TABLE ****//
	//Variable de l'iterateur pour la table de hashage
	printf("\n----> Affichage dico <---- \n");
	GHashTableIter iter;
	char *value;
	int *key;
	g_hash_table_iter_init(&iter,dico); //iterator pour afficher le hashtable
	while (g_hash_table_iter_next (&iter, (gpointer)&key, (gpointer)&value))
	{
		printf("key %d ---> %s \n", *key, value);
	}

	printf("\n----> Test de la Version 1 <---- \n");
	char* resultat = "abaissai";
	printf("Resultat a trouver: %d ---> %s\n", g_str_hash(resultat), resultat);
	char* subtring = version1("dxabaissaiz", dico);


	return EXIT_SUCCESS;
}



