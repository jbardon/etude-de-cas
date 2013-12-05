/**
 * @file GestionEnv.h
 * @brief Gestion de l'environnement graphique et physique du jeu
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Bibliothèque pour la gestion de la SDL, chipmunk, des balles et
 * du tracé des lignes pour sélectionner des balles
 *
 */

#ifndef __GESTION_ENV_H__
#define __GESTION_ENV_H__

#include <chipmunk.h>
#include <SDL_gfxPrimitives.h>
#include <glib.h>

#include <config.h>
#include <Aleatoire.h>
#include <Balle.h>

/* Pour la fonction GestionEnv_afficherMessage (paramètre x) */
#define ALIGN_CENTRE -1

/**
 * @var cpFloat uniteTemps
 * @brief Unite de temps dans chipmunk (en s)
 *
 * Le temps dans chipmunk sera avancé de ce temps
 * à chaque appel à la fonction GestionEnv_evoluer
 *
 */ 
const cpFloat uniteTemps = 1.0/30.0; 

typedef struct _Environnement Environnement;
struct _Environnement {
	cpSpace* espace;
	cpShape* panier[3];

	SDL_Surface* ecran;
	GPtrArray* balles;
	int nbBallesTotal;
	int nbBallesCrees;

	int ajouterBalles;
};

/**
 * Fonction générique pour appliquer plus facilement
 * des fonctions de la bibliothèque balle à toutes
 * les balles de l'environnement
 *
 * @see _Balle_foreach
 */
typedef void (*Balle_Fonction)(Balle*);

extern Environnement* GestionEnv_creerEnvironnement();
extern void GestionEnv_supprimerEnvironnement(Environnement* envJeu);
extern void GestionEnv_rejouer(Environnement* envJeu);

extern void GestionEnv_evoluer(Environnement* envJeu);
extern int GestionEnv_ballesImmobiles(Environnement* envJeu);

extern char* GestionEnv_donnerCaracteresLigne(Environnement* envJeu, int x1, int y1, int x2, int y2);
extern void GestionEnv_afficherMessage(Environnement* envJeu, char* message, int x, int y, int taille);
extern void GestionEnv_viderZoneMessage(Environnement* envJeu);
extern void GestionEnv_effacerPanier(SDL_Surface* ecran);

/* DEBUG */
extern cpShape** donnerSol(Environnement* envJeu);
/* DEBUG */

#endif /* __GESTION_ENV_H__ */
