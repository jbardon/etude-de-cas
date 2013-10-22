#include <SDL/SDL.h>
#include "SDL_Ex.h"

int main(void){
printf("%ll\n",COULEUR_FOND);
	//Cr�� la fen�tre de jeu
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet �tude de cas", NULL);

	SDL_Ex_DessinerLigneHoriz(ecran, 2, 2, 50, 2, 255);
	SDL_Ex_DessinerLigneHoriz(ecran, 2, 2+2, 100, 2, 255);
	
	SDL_Ex_DessinerLigneVert(ecran, 2, 200, 50, 2, 255);
	SDL_Ex_DessinerLigneVert(ecran, 2+2, 200, 100, 2, 255);

	pause();
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
