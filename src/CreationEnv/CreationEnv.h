#ifndef __CREATION_ENV_H__
#define __CREATION_ENV_H__

#include <chipmunk.h>
#include <SDL_Ex.h>
#include <config.h>

extern void CreationEnv_creerPanier(cpSpace* espace, SDL_Surface* surf, int decalage);
extern void CreationEnv_supprimerPanier();

#endif /* __CREATION_ENV_H__ */
