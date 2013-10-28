#ifndef __BALLE_H__
#define __BALLE_H__

#include <stdlib.h>
#include <stdio.h> /* DEBUG */
#include <math.h>
#include <chipmunk.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <config.h>

typedef struct _Balle Balle;
struct _Balle {
	SDL_Surface* ecranJeu;
	SDL_Surface* canvas;
	cpShape* zoneCollision;

	int cx, cy;
	float angle;
	Uint32 couleur;

	char lettre;
};

extern Balle* Balle_creer(cpSpace* espace, SDL_Surface* surf, int cx, int cy, int rayon, Uint32 couleur);
extern void Balle_supprimer(Balle* balle);

extern cpVect Balle_donneCoordonnees(Balle* balle);
extern float Balle_donneAngle(Balle* balle);

extern void Balle_dessiner(Balle* balle);
extern void Balle_effacer(Balle* balle);
extern void Balle_deplacer(Balle* balle);
extern void Balle_rotation(Balle* balle);

#endif /* __BALLE_H__ */
