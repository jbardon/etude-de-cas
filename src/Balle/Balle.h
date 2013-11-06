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

typedef struct _Balle Balle;
struct _Balle {
	SDL_Surface* ecranJeu;
	SDL_Surface* canvas;
	cpShape* zoneCollision;

	int cx, cy;
	Uint32 couleur; /* RRGGBBAA */

	char lettre;
};

extern Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
						  int rayon, Uint32 couleur, char lettre);
extern void Balle_supprimer(Balle* balle);

extern cpVect Balle_donneCoordonnees(Balle* balle);
extern float Balle_donneAngleDeg(Balle* balle);
extern int Balle_estImmobile(Balle* balle);

extern void Balle_afficher(Balle* balle);
extern void Balle_effacer(Balle* balle);

extern void Balle_deplacer(Balle* balle);
extern SDL_Surface* Balle_rotation(Balle* balle);

#endif /* __BALLE_H__ */
