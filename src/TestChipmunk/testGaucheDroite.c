/**
 * @file testGaucheDroite.c
 * @brief Affichage d'un panier avec une balle qui tombe
 * ou le sol n'est pas plat
 *
 * Test le moteur physique chipmunk et le débordement
 * des coordonnées lorsque la vitesse de la balle est trop grande
 *
 * La balle débute en haut à gauche et
 * le sol oblique amène la balle en bas à droite
 *
 */

#include <stdio.h>
#include <math.h>

#include <unistd.h> //pause
#include <GestionEnv.h>
#include <Balle.h>

int main(void){

	Environnement* envJeu = GestionEnv_creerEnvironnement();

/* DEBUT TEST */

	cpShape** panier = donnerSol(envJeu); //!\\ Fonction uniquement pour débugger
	*panier = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(0,300), cpv(LARGUEUR_ECRAN,50), 0);
	cpShapeSetFriction(*panier, 1);
	cpShapeSetElasticity(*panier, 1);
	cpSpaceAddShape(envJeu->espace, *panier);

	Balle* balle = Balle_creer(envJeu->ecran, envJeu->espace, cpv(100,0), cpvzero, 50, 0x00FF00FF, 'A');

/* FIN TEST */

	SDL_Flip(envJeu->ecran);

	cpFloat timeStep = 1.0/60.0;
	for(cpFloat time = 0; time < 10; time += timeStep){

		cpVect pos = Balle_donneCoordonnees(balle);
		float ang = Balle_donneAngleDeg(balle);

		printf(
			"Temps %5.2f balle(%5.2f, %5.2f) angle %f\n",
			time, pos.x, pos.y, ang
		);

		cpSpaceStep(envJeu->espace, timeStep);
		Balle_deplacer(balle);
		SDL_Flip(envJeu->ecran);
	}

	pause();

	Balle_supprimer(balle);
	GestionEnv_supprimerEnvironnement(envJeu);

	return 0;
}
