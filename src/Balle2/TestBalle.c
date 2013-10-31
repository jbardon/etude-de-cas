#include <unistd.h> //pause
#include <Balle.h>

int main(void){

	//Créé la fenêtre de jeu
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);

	//Créé un espace vide et spécifie sa gravitée
	cpSpace* space = cpSpaceNew();
	cpVect gravity = cpv(0, -100);  
	cpSpaceSetGravity(space, gravity);

/* DEBUT TEST */

	Balle* balle = Balle_creer(ecran, space, 100, 100, 50, SDL_MapRGB(ecran->format, 0, 255, 255), 'A');

	sleep(1);
	Balle_effacer(balle);

	//Par défaut effacer ne met pas à jour l'affichage
	//Sinon la balle clignote lors du déplacement
	SDL_Flip(balle->ecranJeu); 

	sleep(1);
	cpBodySetPos(cpShapeGetBody(balle->zoneCollision), cpv(200, 200));
	Balle_deplacer(balle);

/* FIN TEST */

	pause();

	//Détruit la fenêtre
	Balle_supprimer(balle);
	SDL_FreeSurface(ecran);
	SDL_Quit();

	//Détruit l'espace physique
	cpSpaceFree(space);

	return 0;
}
