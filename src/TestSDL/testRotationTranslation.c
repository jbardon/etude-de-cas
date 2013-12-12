/**
 * @file testRotationTranslation.c
 * @brief Test sur la translation et la rotation d'une surface SDL
 *
 * Déplacement et rotation d'une surface SDL 
 *
 * Ce test met en évidence le problème suivant :
 *		Une surface est carré donc lors d'une rotation elle peut se transformer
 *		(à l'extrême en losange) qui dépasse la figure d'origine de tout les côtés
 *
 *  	Il faut donc rogner le résultat de la rotation et recalculer sa position sur l'écran
 *		L'exemple montre dans l'ordre (à l'écran): surface originale, rotation rognée, rotation
 *
 */

#include <stdio.h>
#include <unistd.h> //pause

#include <config.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>

int main(void){

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);

/* DEBUT TEST */

	// Cercle d'origine
	SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, 0, 0, 0, 0);
	filledCircleColor(s, 50 , 50, 50, SDL_MapRGB(ecran->format, 255, 0, 255));
	lineColor(s, 0, 20, 100, 20, 0x0000FFFF);

	SDL_Rect position = { 50, 50 };
	SDL_BlitSurface(s, NULL, ecran, &position); 
	printf("Taille surface origine: h(%d) w(%d)\n", s->h, s->w);

	// Rotation de 45 deg et affichage
	SDL_Surface* s2 = rotozoomSurface(s, 45, 1, 0); 
	position.x += 300;
	SDL_BlitSurface(s2, NULL, ecran, &position);  
	printf("Taille surface après rotation: h(%d) w(%d)\n", s2->h, s2->w);
 
	// Correction de la position et rognage
	int deltaH = s2->h - s->h;
	int deltaW = s2->w - s->w;

	SDL_Rect clip = { deltaW/2, deltaH/2, s->h, s->w };
	position.x -= 150; 
	SDL_BlitSurface(s2, &clip, ecran, &position);

	SDL_Flip(ecran);

/* FIN TEST */

	pause();

	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
