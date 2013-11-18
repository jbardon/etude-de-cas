#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../main/config.h"
#include "AlgoRecherche.h"

int main()
{	

	//création de la table de hashage pour ranger le dico
	printf("\n----> Test chargerDico <---- \n");
	GHashTable* dico = chargerDico("dicofinal.txt");

	//**** AFFICHAGE DE LA TABLE ****//
	//Variable de l'iterateur pour la table de hashage
	printf("\n----> Affichage dico <---- \n");
	GHashTableIter iter;
	char *value;
	int *key;
	g_hash_table_iter_init(&iter,dico); //iterator pour afficher le hashtable

	while (g_hash_table_iter_next (&iter, (gpointer)&key, (gpointer)&value))
	{
//		printf("key %d ---> %s \n", *key, value);
	}

	printf("\n----> Test de la Version 1 <---- \n");
	char* resultat = "ABAISSAI";
	char* result = (char*) calloc(strlen(resultat) + 1, sizeof(char));

	for(int i = 0; i < strlen(resultat); i++){
	  result[i] = tolower(resultat[i]);
	}

	printf("Resultat a trouver: %d ---> %s %d\n", g_str_hash(result), result, strlen(result));

	char* substring = version1(result, dico);
	if(substring){
		printf("Chaine la plus longue : %s, vous marquez %d points \n", substring, strlen(substring)); 
	}
	else {
		printf("Aucun mot trouvé\n");
	}


	printf("\n----> Test de la Version 2 <---- \n");
	char* resultat2 = "abcde";
	char* substring2 = version2(resultat2, dico);

	free(result);
	g_hash_table_destroy(dico);	

	return EXIT_SUCCESS;
}



