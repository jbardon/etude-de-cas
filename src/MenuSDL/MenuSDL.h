#ifndef __MENU_SDL_H__
#define __MENU_SDL_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <config.h>

typedef struct _Solution Solution;
struct _Solution{
	char* mot;
	int score;
};

extern Solution* Solution_creer(char* mot, int score);
extern void Solution_supprimer(Solution* motTrouve);
extern char* Solution_afficherScore(Solution* motTrouve);

extern SDL_Surface* MenuSDL_creer(SDL_Surface* ecran, char* lettres, Solution** motsTrouves, int nbMotsTrouves);

#endif /* __MENU_SDL_H__ */
