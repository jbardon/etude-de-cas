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

void supp(GHashTable* table);
extern char* RechercheAnagramme(char *chaine, GHashTable* table); //recherche l'anagramme d'un mot
