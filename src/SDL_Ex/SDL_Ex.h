#ifndef __SDL_EX_H__
#define __SDL_EX_H__

#include <SDL/SDL.h>
#include <config.h>

typedef struct _SDL_Ex_Point SDL_Ex_Point;
struct _SDL_Ex_Point {
	int x, y;
};

//Repr�sente un cercle
typedef struct _SDL_Ex_Cercle SDL_Ex_Cercle;
struct _SDL_Ex_Cercle {
	int cx, cy; //Coordonn�es du centre du cercle
	int rayon;
	Uint32 couleur;
};

extern unsigned long SDL_Ex_Couleur(int R, int G, int B);
extern SDL_Ex_Point point(int x, int y);

extern void SDL_Ex_DessinerLigneHoriz(SDL_Surface* surf, int x, int y, int l, int e, Uint32 coul);
extern void SDL_Ex_DessinerLigneVert(SDL_Surface* surf, int x, int y, int h, int e, Uint32 coul);
					
extern void SDL_Ex_DessinerLigne(SDL_Surface* surf, SDL_Ex_Point debut,
									  SDL_Ex_Point fin, Uint32 coul);
									  
extern void SDL_Ex_DessinerCercle(SDL_Surface* surf, const SDL_Ex_Cercle* cercle);
extern void SDL_Ex_DeplacerCercle(SDL_Surface* surf, SDL_Ex_Cercle* cercle, SDL_Ex_Point dest, Uint32 couleurFond);

#endif /* __SDL_EX_H__ */
