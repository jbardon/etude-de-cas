/**
* @file AlgoRecherche.h
* @brief Contient l'ensemble des algos sur les chaine de caractere
*/

#include <ctype.h>
#include <glib.h>
#include <stdio.h> /* DEBUG */
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

extern GHashTable* chargerDicoV1_V2(char* nomFichier);
extern GHashTable* chargerDicoV3(char* nomFichier);

extern char* version1(GHashTable* table, char* chaine);
extern char* version2(GHashTable* table, char* chaine);
extern char* version3(GHashTable* table, char* chaine);

extern char* RechercheAnagramme(GHashTable* table, char *chaine); //recherche l'anagramme d'un mot
extern void supprimerDicoV3(GHashTable* table);

