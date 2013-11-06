#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

/* DEBUT TEST */
// Créé une balle si une ligne est en collision avec elle

	Balle* balle = Balle_creer(ecran, espace, cpv(LARGUEUR_ECRAN/2, 50), cpvzero, 50, 0x00FF00FF, ' ');
	cpBodySetVelLimit(cpShapeGetBody(balle->zoneCollision), 30);

	// Extrémités de la ligne
	const int x1 = 0, x2 = LARGUEUR_ECRAN;
	const int y1 = 0, y2 = HAUTEUR_ECRAN;
	
	cpFloat temps = 0, uniteTemps = 1.0/60.0;
	for(; temps < 12; temps += uniteTemps){

	    // Vérifie si la balle est touchée par la ligne
		int touche =  cpShapeSegmentQuery(
						   balle->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		printf("Temps %2.2f touche %d\n", temps, touche);		

		cpSpaceStep(espace, uniteTemps);	
		Balle_deplacer(balle);

		// Dessine la ligne
		lineColor(ecran, x1, y1, x2, y2, 0x00FFFF);

		SDL_Flip(ecran);
	}

/* FIN TEST */

	pause();

	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
