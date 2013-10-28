#include <stdio.h>
#include <math.h>

#include <unistd.h> //pause
#include <GestionEnv.h>
#include <Balle.h>

int main(void){

  //Créé la fenêtre de jeu
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
  SDL_FillRect(ecran, NULL, COULEUR_FOND);
  SDL_WM_SetCaption("Projet étude de cas", NULL);

	SDL_Surface* s = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, 0, 0, 0, 0);
	SDL_FillRect(s, NULL, 0x00000000);
	SDL_Rect position = { 50, 50 };
	filledCircleColor(s, 50 , 50, 50, SDL_MapRGB(ecran->format, 255, 0, 255));
	lineColor(s, 0, 20, 100, 20, 0x0000FFFF);
	SDL_BlitSurface(s, NULL, ecran, &position);  

  SDL_Flip(ecran);

	s = rotozoomSurface(s, 90, 1, 0);
	SDL_Rect position2 = { 200, 150 };
	SDL_BlitSurface(s, NULL, ecran, &position2);  
	//filledCircleColor(s, 50 , 50, 50, SDL_MapRGB(ecran->format, 255, 0, 255));

  SDL_Flip(ecran);

	pause();
	
  //Détruit la fenêtre
  SDL_FreeSurface(ecran);
  SDL_Quit();

  return 0;
}
