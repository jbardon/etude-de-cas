#include <unistd.h> //pause
#include <Balle.h>

int main(void){

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);

	cpSpace* espace = cpSpaceNew();
	cpVect gravite = cpv(0, -100);  
	cpSpaceSetGravity(espace, gravite);

/* DEBUT TEST */

TTF_Init();
	for(char l = 65; l < 91; l++){
		Balle* balle = Balle_creer(ecran, espace, cpv(100,100), cpvzero, 50, 0x00FF00FF, l);
		SDL_Flip(ecran);
		getchar();	
		Balle_supprimer(balle);
	}
TTF_Quit();

/* FIN TEST */

	cpSpaceFree(espace);
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
