#ifndef __GESTION_ENV_H__
#define __GESTION_ENV_H__

#include <chipmunk.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <config.h>

extern void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf, int decalage);
extern void GestionEnv_supprimerPanier();

#endif /* __GESTION_ENV_H__ */
