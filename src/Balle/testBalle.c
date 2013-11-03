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

	Balle* balle = Balle_creer(ecran, espace, cpv(100,100), cpvzero, 50, 0x00FF00FF, 'A');

	printf("Balle_creer...fait\n");
	sleep(1);
	//balle->cx+=150;
	Balle_effacer(balle);

	//Par défaut effacer ne met pas à jour l'affichage
	//Sinon la balle clignote lors du déplacement
	SDL_Flip(balle->ecranJeu); 
	printf("Balle_effacer...fait\n");

	sleep(1);
	cpBodySetPos(cpShapeGetBody(balle->zoneCollision), cpv(200, 200));
	//Balle_deplacer(balle);

/* FIN TEST */

	pause();

	Balle_supprimer(balle);
	cpSpaceFree(espace);
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
