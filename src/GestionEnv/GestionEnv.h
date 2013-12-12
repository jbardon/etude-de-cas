/**
 * @file GestionEnv.h
 * @brief Gestion de l'environnement graphique et physique du jeu
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

/** 
	@brief Centrer le message sur la surface
	@see GestionEnv_afficherMessage
 */
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

/**
 * @struct Environnement
 * @brief Représente l'environnement de jeu
 *
 * L'environnement est physique (chipmunk) et graphique (SDL)
 * Il s'occupe de gérer l'évolution des balles dans le panier
 *
 */
//typedef struct _Environnement Environnement; /* Documentation plus propre */
//struct _Environnement {
typedef struct {
	cpSpace* espace; 	/*!< Espace physique (chipmunk) */
	cpShape* panier[3]; /*!< 3 éléments du physique du panier: sol, mur gauche et mur droit */

	SDL_Surface* ecran; /*!< Ecran principal de la fenêtre (SDL) */
	GPtrArray* balles;  /*!< Balles présentes dans l'environnement */
	int nbBallesTotal;  /*!< Nombre de balles demandées */
	int nbBallesCrees;  /*!< Nombre de balles créées */

	int ajouterBalles;  /*!< Booléen qui indique si toutes les balles ont été ajoutées */
} Environnement;

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
