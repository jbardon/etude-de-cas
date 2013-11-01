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
//La balle débute en haut à droite
//Le sol oblique amène la balle en bas à gauche

	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,50), cpv(LARGUEUR_ECRAN,300), 0);
	cpShapeSetFriction(panier[0], 1);
	cpShapeSetElasticity(panier[0], 1);
	cpSpaceAddShape(espace, panier[0]);

	Balle* balle = Balle_creer(ecran, espace, cpv(LARGUEUR_ECRAN - 100,0), cpvzero, 50, 0x00FF00FF, 'A');

/* FIN TEST */

	SDL_Flip(ecran);

	//Evolution des objets de l'espace dans le temps
	cpFloat timeStep = 1.0/60.0;
	for(cpFloat time = 0; time < 8; time += timeStep){
		cpVect pos = Balle_donneCoordonnees(balle);
		float ang = Balle_donneAngleDeg(balle);

		printf(
		"Temps %5.2f balle(%5.2f, %5.2f) angle %f\n",
		time, pos.x, pos.y, ang
		);

		cpSpaceStep(espace, timeStep);
		Balle_deplacer(balle);
	}

	pause();

	//Détruit la fenêtre
	Balle_supprimer(balle);
	GestionEnv_supprimerPanier();

	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
