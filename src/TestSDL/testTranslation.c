#include <stdio.h>
#include <unistd.h> //pause

#include <config.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

int main(void){

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);

/* DEBUT TEST */

	//Surface initiale
	SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, 0, 0, 0, 0);
	SDL_FillRect(s, NULL, 0x00000000);
	SDL_Rect position = { 50, 50 };
	filledCircleColor(s, 50 , 50, 50, SDL_MapRGB(ecran->format, 255, 0, 255));
	SDL_BlitSurface(s, NULL, ecran, &position);  

	//Translation de vecteur v(150,100)
	position.x += 150;
	position.y += 100;
	SDL_BlitSurface(s, NULL, ecran, &position);  
	
	SDL_Flip(ecran);

/* FIN TEST */

	pause();
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
