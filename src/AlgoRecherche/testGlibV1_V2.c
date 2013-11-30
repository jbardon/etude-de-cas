#include <stdio.h>
#include <stdlib.h>
#include <config.h>

#include "AlgoRecherche.h"

//gcc testGlibV1_V2.c -std=c99 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -lglib-2.0 && ./a.out
int main()
{	

	//Données à entrer dans la table
	char* data[]={
		"Hello",
		"World"
	};
	const int sizeData = sizeof(data)/sizeof(char*);

	int hash[2];
	for(int i = 0; i < sizeData; i++)
	{
		hash[i] = g_str_hash(data[i]);
	} 

	// Insertion des données dans une table
	GHashTable* table = g_hash_table_new(g_int_hash, g_int_equal);

	for(int i = 0; i < sizeData; i++)
	{
		g_hash_table_insert(table, &hash[i], g_strdup(data[i]));
	}

	// Affichage de la table
	GHashTableIter iter;
	char *value;
	int *key;

	g_hash_table_iter_init(&iter, table); //iterator pour afficher le hashtable
	while(g_hash_table_iter_next(&iter, (gpointer)&key, (gpointer)&value))
	{
		printf("key %d ---> %s \n", *key, value);
	}
	
	// Recherche un élément dans la table
	char* mot = "World";
	int hashMot = g_str_hash(mot);
	printf("mot: %s ---> hashMot: %d \n", mot, hashMot);

	void* motTrouve = g_hash_table_lookup(table, &hashMot);
	if(motTrouve)
	{
		printf("Trouvé: %s\n", (char*)motTrouve);
	}	
	else
	{
		printf("Pas trouvé\n");	
	}

	g_hash_table_destroy(table);

	return EXIT_SUCCESS;
}

