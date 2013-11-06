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

	cpShape* sol = cpSegmentShapeNew(espace->staticBody, cpv(0,10), cpv(LARGUEUR_ECRAN,10), 0);
	cpShapeSetFriction(sol, FRICTION);
	cpShapeSetElasticity(sol, REBOND);
	cpSpaceAddShape(espace, sol);

/* DEBUT TEST */

TTF_Init();
	Balle* balle = Balle_creer(ecran, espace, cpv(100,100), cpvzero, 50, 0x00FF00FF, 'A');
TTF_Quit();

	//Affichage de la balle
	SDL_Flip(ecran);
	printf("Balle_creer...fait\n");

	//Effacement de la balle
	sleep(1);
	Balle_effacer(balle);
	//Par défaut effacer ne met pas à jour l'affichage
	//Sinon la balle clignote lors du déplacement
	SDL_Flip(ecran); 
	printf("Balle_effacer...fait\n");

	//Deplacement de la balle
	sleep(1);
	cpBodySetPos(cpShapeGetBody(balle->zoneCollision), cpv(200, 200));
	Balle_deplacer(balle);
	SDL_Flip(ecran);
	printf("Balle_deplacer...fait\n");

	//Balle immobile
	cpFloat temps = 0, uniteTemps = 1.0/60.0;
	do {		
		cpVect pos = Balle_donneCoordonnees(balle);

		printf(
			"Temps %5.2f balle(%5.2f, %5.2f)\n",
			temps, pos.x, pos.y
		);

		cpSpaceStep(espace, uniteTemps);
		Balle_deplacer(balle);
		SDL_Flip(ecran);
		
		temps += uniteTemps;
	}
	while(!Balle_estImmobile(balle));

	printf("Balle_estImmobile...fait\n");

/* FIN TEST */

	pause();

	Balle_supprimer(balle);

	cpSpaceFree(espace);
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
