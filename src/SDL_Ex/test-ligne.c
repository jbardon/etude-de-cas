#include <SDL/SDL.h>
#include "SDL_Ex.h"

int main(void){

	//Créé la fenêtre de jeu
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);


	SDL_Ex_DessinerLigneHoriz(ecran, 150, 100, 100, 49, 1500);
	SDL_Ex_DessinerLigneHoriz(ecran, 250, 100, 100, 50, 60000);
	SDL_Ex_DessinerLigneHoriz(ecran, 350, 100, 100, 51, 95000);
	
	SDL_Ex_DessinerLigneVert(ecran, 50, HAUTEUR_ECRAN - 50, 100, 49, 1500);
	SDL_Ex_DessinerLigneVert(ecran, 50, HAUTEUR_ECRAN - 50 - 100, 100, 50, 60000);
	SDL_Ex_DessinerLigneVert(ecran, 50, HAUTEUR_ECRAN - 50 - 100 - 100, 100, 51, 95000);

	SDL_Flip(ecran);
	pause();
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
