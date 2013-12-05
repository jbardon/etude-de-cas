/**
 * @file testCollision.c
 * @brief Test sur la collision entre une balle 
 * et une ligne avec chipmunk
 *
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Une ligne est tracée du coin supérieur gauche de l'écran 
 * au coin inférieur droit. Une balle au centre de l'écran tombe 
 * et passe à travers de la ligne qui détecte la collision 
 * qui est affichée dans le terminal
 *
 */

#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

int main(void){

	Environnement* envJeu = GestionEnv_creerEnvironnement();

/* DEBUT TEST */

	Balle* balle = Balle_creer(envJeu->ecran, envJeu->espace, cpv(LARGUEUR_ECRAN/2, 50), cpvzero, 50, 0x00FF00FF, ' ');
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

		cpSpaceStep(envJeu->espace, uniteTemps);	
		Balle_deplacer(balle);

		// Dessine la ligne
		lineColor(envJeu->ecran, x1, y1, x2, y2, 0x00FFFF);

		SDL_Flip(envJeu->ecran);
	}

/* FIN TEST */

	pause();
	GestionEnv_supprimerEnvironnement(envJeu);

	return 0;
}
