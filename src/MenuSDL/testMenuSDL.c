#include <stdio.h>
#include <unistd.h>

#include <MenuSDL.h>
#include <config.h>

int main(void){

	/* Init */
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);

	if(ecran){
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
		SDL_WM_SetCaption("Projet etude de cas", NULL);
	}

	/* Déclaration des scores */
	Solution* motsTrouves[3] = {
		Solution_creer("KJGIHBK", 15),
		Solution_creer("JKHIKB", 1),
		Solution_creer("VNBXCWDS", 99)
	};	

	/* Créé et affiche le menu */
	SDL_Surface* menu = MenuSDL_creer(ecran, "AAJKNBKI", motsTrouves, 3);
	SDL_Rect pos = { 0, 0 };
	SDL_BlitSurface(menu, NULL, ecran, &pos);
	SDL_Flip(ecran);

	SDL_FreeSurface(menu);

	pause();

	/* Quit */
	SDL_FreeSurface(ecran);
	TTF_Quit();
	SDL_Quit();

  return 0;
}
