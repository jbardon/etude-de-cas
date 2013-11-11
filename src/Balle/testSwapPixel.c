/**
 * @file testSwapPixel.c
 * @brief Affichage d'une surface et modification de sa couleur (avec SDL)
 * @author Jérémy.B
 * @version 1.0
 *
 * Test de recherches pour la bibliothèque Balle afin d'effacer
 * la balle efficacement car avec la rotation des petites
 * trace sont visibles.
 *
 * Change la couleur des pixels d'une surface un à un.
 *
 */

#include <stdio.h>
#include <SDL.h>

int main(void){

	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Surface* s = SDL_CreateRGBSurface(SDL_SRCALPHA, 2, 2, 32, 0, 0, 0, 0);
	Uint32 *pixels = (Uint32 *)s->pixels;

	for(int i = 0; i < s->w * s->h; i++){

		int x = i / s->w, y = i % s->w;
		pixels[i] = 25;
		Uint32 c = pixels[i];

		printf("(%d, %d): %d\n", x, y, c);	
	}

	SDL_Quit();

	return 0;
}
