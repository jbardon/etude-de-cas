/**
 * @file Balle.h
 * @brief Gestion d'une balle d'un point de vue graphique et physique
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Bibliothèque pour la gestion d'une balle avec la SDL et chipmunk
 *
 */

#ifndef __BALLE_H__
#define __BALLE_H__

#include <stdlib.h>
#include <stdio.h> /* DEBUG */
#include <math.h>
#include <chipmunk.h>

#include <SDL_ttf.h>
#include <SDL_image.h>

#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include <config.h>

/**
 * @struct Balle
 * @brief Objet représentant une balle 
 *
 * Une balle est représentée graphiquement (avec la SDL)
 * et physiquement (avec chipmunk) 
 *
 */
//typedef struct _Balle Balle; /* Documentation plus propre */
//struct _Balle {
typedef struct {
	SDL_Surface* ecranJeu;  /*!< Ecran principal de la fenêtre SDL */
	SDL_Surface* canvas;    /*!< Surface ou la balle est dessinée */

	cpSpace* espaceJeu;     /*!< Espace physique (chipmunk) ou est la balle */
	cpShape* zoneCollision; /*!< Zone de collision de la balle (chipmunk) */

	int cx;                 /*!< Abscisse du centre de la balle */
    int cy;					/*!< Ordonnée du centre de la balle */
	Uint32 couleur;			/*!< Couleur de la balle sous la forme 0xRRGGBBAA */

	char lettre;			/*!< Lettre liée à la balle (dessinée au centre) */
} Balle;


extern Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
						  int rayon, Uint32 couleur, char lettre);
extern void Balle_supprimer(Balle* balle);

extern cpVect Balle_donneCoordonnees(Balle* balle);
extern float Balle_donneAngleDeg(Balle* balle);
extern int Balle_estImmobile(Balle* balle);

extern void Balle_afficher(Balle* balle);
extern void Balle_effacer(Balle* balle);
extern void Balle_deplacer(Balle* balle);

#endif /* __BALLE_H__ */

