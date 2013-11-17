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

#include <time.h>

#include <chipmunk.h>
#include <SDL_gfxPrimitives.h>
#include <config.h>
#include <Balle.h>

/**
 * @var cpFloat uniteTemps
 * @brief Unite de temps dans chipmunk (en s)
 *
 * Le temps dans chipmunk sera avancé de ce temps
 * à chaque appel à la fonction GestionEnv_evoluer
 *
 */ 
const cpFloat uniteTemps = 1.0/60.0; 

/**
 * Fonction générique pour appliquer plus facilement
 * des fonctions de la bibliothèque balle à toutes
 * les balles de l'environnement
 *
 * @see _Balle_foreach
 */
typedef void (*Balle_Fonction)(Balle*);

extern cpSpace* GestionEnv_initChipmunk();
extern SDL_Surface* GestionEnv_initSDL();

extern void GestionEnv_evoluer();

extern void GestionEnv_quitChipmunk();
extern void GestionEnv_quitSDL();

extern void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf);
extern void GestionEnv_supprimerPanier();

extern void GestionEnv_creerBalles(int nbBalles);
extern void GestionEnv_supprimerBalles();

extern int GestionEnv_ballesImmobiles();

extern char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2);
extern void GestionEnv_afficherMessage(char* message, int x, int y, int taille);
extern void GestionEnv_viderZoneMessage();

/* DEBUG */
extern cpShape** donnerSol();
/* DEBUG */

#endif /* __GESTION_ENV_H__ */
