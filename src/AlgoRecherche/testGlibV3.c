/**
* @file testGlibV1_V2.c
* @brief Permet de tester la GLib et la table de Hashage pour la version 3
* @author Mickael.A
*
* Test qui test la GLib pour la troisième version des algos
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

//gcc testGlibV3.c -std=c99 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -lglib-2.0 && ./a.out

void insert(GHashTable* table, char* data, char* key)
{
	GSList* anag = (GSList*) g_hash_table_lookup(table, key);	
	anag = g_slist_append(anag, data);
	g_hash_table_replace(table, key, anag);
}

int main()
{	
	//Données à entrer dans la table
	char* data[]={
		"bcdea",
		"cdeab",
		"deabc",
		"eabcd",
		"salut",
	};
	const int sizeData = sizeof(data)/sizeof(char*);

	char* hash[]={
		"abcde",
		"abcde",
		"abcde",
		"abcde",
		"alstu",
	};

	// Insertion des données dans une table
	GHashTable* table = g_hash_table_new(g_str_hash, g_str_equal);

	for(int i = 0; i < sizeData; i++)
	{
		insert(table, data[i], hash[i]);
	}
	
	// Recherche un élément dans la table
	void* trouve = g_hash_table_lookup(table, "abcde");
	if(trouve)
	{
		GSList* anag = (GSList*) trouve;
		for (GSList* iterator = anag; iterator; iterator = iterator->next) 
		{
			printf("Current item is '%s'\n", (char*)iterator->data);
		}
	}	
	else 
	{
		printf("Pas trouvé\n");	
	}

	g_hash_table_destroy(table);

	return EXIT_SUCCESS;
}

