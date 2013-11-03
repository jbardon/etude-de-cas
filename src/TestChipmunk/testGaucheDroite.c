#include <stdio.h>
#include <math.h>

#include <unistd.h> //pause
#include <GestionEnv.h>
#include <Balle.h>

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);

/* DEBUT TEST */
// La balle débute en haut à gauche
// Le sol oblique amène la balle en bas à droite

	cpShape** panier = donnerSol(); //!\\ Fonction uniquement pour débugger
	*panier = cpSegmentShapeNew(espace->staticBody, cpv(0,300), cpv(LARGUEUR_ECRAN,50), 0);
	cpShapeSetFriction(*panier, 1);
	cpShapeSetElasticity(*panier, 1);
	cpSpaceAddShape(espace, *panier);

	Balle* balle = Balle_creer(ecran, espace, cpv(100,0), cpvzero, 50, 0x00FF00FF, 'A');

/* FIN TEST */

	SDL_Flip(ecran);

	cpFloat timeStep = 1.0/60.0;
	for(cpFloat time = 0; time < 10; time += timeStep){

		cpVect pos = Balle_donneCoordonnees(balle);
		float ang = Balle_donneAngleDeg(balle);

		printf(
			"Temps %5.2f balle(%5.2f, %5.2f) angle %f\n",
			time, pos.x, pos.y, ang
		);

		cpSpaceStep(espace, timeStep);
		Balle_deplacer(balle);
		SDL_Flip(ecran);
	}

	pause();

	Balle_supprimer(balle);
	GestionEnv_supprimerPanier();

	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
