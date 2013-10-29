#include <stdio.h>
#include <unistd.h> //pause

#include <config.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>

int main(void){

  //Créé la fenêtre de jeu
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
  SDL_FillRect(ecran, NULL, COULEUR_FOND);
  SDL_WM_SetCaption("Projet étude de cas", NULL);

	SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, 0, 0, 0, 0);
	SDL_Rect position = { 50, 50 };
	filledCircleColor(s, 50 , 50, 50, SDL_MapRGB(ecran->format, 255, 0, 255));
	lineColor(s, 0, 20, 100, 20, 0x0000FFFF);
	SDL_BlitSurface(s, NULL, ecran, &position);  

  SDL_Flip(ecran);

	SDL_Surface* s2 = rotozoomSurface(s, 90, 1, 0); 
	position.x += 150;
	SDL_BlitSurface(s2, NULL, ecran, &position);  

  SDL_Flip(ecran);

	//!\\ Il faut obligatoirement donner la surface d'origine s et non s2
	//!\\ Sinon ça ne marche pas ...
	SDL_Surface* s3 = rotozoomSurface(s, -90, 1, 0); 
	position.x += 150;
	SDL_BlitSurface(s3, NULL, ecran, &position); 

  SDL_Flip(ecran);

	pause();
	
  //Détruit la fenêtre
  SDL_FreeSurface(ecran);
  SDL_Quit();

  return 0;
}
