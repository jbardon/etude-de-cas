/**
 * \file Balle.h
 * \brief Gestion d'une balle d'un point de vue graphique et physique
 * \author Jérémy.B
 * \version 1.0
 * \date 10 novembre 2013
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
 * \struct Balle
 * \brief Objet représentant une balle 
 *
 * Une balle est représentée graphiquement (avec la SDL)
 * et physiquement (avec chipmunk) 
 *
 */
typedef struct _Balle Balle;
struct _Balle {
	SDL_Surface* ecranJeu;  /*!< Ecran principal de la fenêtre SDL */
	SDL_Surface* canvas;    /*!< Surface ou la balle est dessinée */

	cpSpace* espaceJeu;     /*!< Espace physique (chipmunk) ou est la balle */
	cpShape* zoneCollision; /*!< Zone de collision de la balle (chipmunk) */

	int cx, cy;             /*!< Coordonnées du centre de la balle */
	Uint32 couleur;			/*!< Couleur de la balle sous la forme 0xRRGGBBAA */

	char lettre;			/*!< Lettre liée à la balle (dessinée au centre) */
};

/**
 * \fn Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
 *						  int rayon, Uint32 couleur, char lettre)
 *
 * \brief Fonction de création d'une nouvelle balle
 *
 * \param surf Ecran principal de la fenêtre SDL
 * \param espace Espace physique (chipmunk) ou est la balle
 * \param centre Coordonnées du centre de la balle
 * \param direction Direction initiale vers laquelle la balle sera lancée
 * \param rayon Rayon de la balles
 * \param couleur Couleur de la balle sous la forme 0xRRGGBBAA
 * \param lettre Lettre liée à la balle (dessinée au centre)
 *
 * \return Nouvelle balle créée ou NULL si la création a échoué
 */
extern Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
						  int rayon, Uint32 couleur, char lettre);

/**
 * \fn void Balle_supprimer(Balle* balle)
 * \brief Fonction de suppression d'une balle au niveau graphique et physique
 *
 * \param balle Balle à supprimer
 */
extern void Balle_supprimer(Balle* balle);


/**
 * \fn cpVect Balle_donneCoordonnees(Balle* balle)
 * \brief Accesseur qui renvoie les coordonnées d'une balle
 *
 * \param balle Balle dont on veut les coordonnées
 * \return Coordonnées de la balle
 */
extern cpVect Balle_donneCoordonnees(Balle* balle);

/**
 * \fn float Balle_donneAngleDeg(Balle* balle)
 * \brief Accesseur sui renvoie l'angle de balle
 *
 * \param balle Balle dont on veut l'angle
 * \return Angle de la balle
 */
extern float Balle_donneAngleDeg(Balle* balle);

/**
 * \fn int Balle_estImmobile(Balle* balle)
 * \brief Fonction qui renvoie 0 si la balle est en mouvement, 1 sinon
 *
 * \param balle Balle dont on veut savoir si elle est immobile
 * \return 0 si la balle est en mouvement, 1 sinon
 */
extern int Balle_estImmobile(Balle* balle);

/**
 * \fn void Balle_afficher(Balle* balle)
 * \brief Fonction qui affiche une balle sur la fenêtre SDL donnée 
 * lors de la création de la balle
 *
 * \param balle Balle que l'on veut afficher
 */
extern void Balle_afficher(Balle* balle);

/**
 * \fn void Balle_effacer(Balle* balle)
 * \brief Fonction qui efface une balle sur la fenêtre SDL donnée 
 * lors de la création de la balle
 *
 * \param balle Balle que l'on veut effacer
 */
extern void Balle_effacer(Balle* balle);

/**
 * \fn void Balle_deplacer(Balle* balle)
 * \brief Fonction qui efface la balle, met à jour les coordonnées
 *  de la balle avec chipmunk et affiche la balle
 *
 * \param balle Balle que l'on veut deplacer
 */
extern void Balle_deplacer(Balle* balle);

#endif /* __BALLE_H__ */

