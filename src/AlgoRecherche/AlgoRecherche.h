#include <stdio.h> /* DEBUG */
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <glib.h>

extern GHashTable* chargerDico(char* nomFichier);
extern char* version1(char* chaine, GHashTable* table);

extern void supprimer_retour_ligne(char *chaine);
