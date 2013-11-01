#ifndef __GESTION_ENV_H__
#define __GESTION_ENV_H__

#include <time.h>

#include <chipmunk.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <config.h>
#include <Balle.h>

cpSpace* espace;
SDL_Surface* ecran;

cpShape* panier[3];
Balle** balles;

extern cpSpace* GestionEnv_initChipmunk();
extern SDL_Surface* GestionEnv_initSDL();

extern void GestionEnv_quitChipmunk();
extern void GestionEnv_quitSDL();

extern void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf);
extern void GestionEnv_supprimerPanier();

extern void GestionEnv_creerBalles(int nbBalles);
extern void GestionEnv_supprimerBalles();

#endif /* __GESTION_ENV_H__ */
