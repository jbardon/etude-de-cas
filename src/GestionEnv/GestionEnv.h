#ifndef __GESTION_ENV_H__
#define __GESTION_ENV_H__

#include <time.h>

#include <chipmunk.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <config.h>
#include <Balle.h>

const cpFloat uniteTemps = 1.0/60.0;

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

extern char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2);

/* DEBUG */
extern cpShape** donnerSol();
/* DEBUG */

#endif /* __GESTION_ENV_H__ */
