#ifndef __BALLE_H__
#define __BALLE_H__

#include <stdlib.h>
#include <stdio.h> /* DEBUG */
#include <chipmunk.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <config.h>

typedef struct _Balle Balle;
struct _Balle {
	cpShape* zoneCollision;
	SDL_Surface* ecran;
	int cx, cy;
	Uint32 couleur;
};

extern Balle* Balle_creer(cpSpace* espace, SDL_Surface* surf, int cx, int cy, int rayon, Uint32 couleur);
extern void Balle_supprimer(Balle* balle);

extern cpVect Balle_donneCoordonnees(Balle* balle);
extern void Balle_deplacer(Balle* balle);

#endif /* __BALLE_H__ */
