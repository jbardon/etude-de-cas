/**
 * @file testBalle.c
 * @brief Fait evoluer un environnement avec la bibliothèque GestionEnv 
 * @author Jérémy.B
 * @version 1.0
 *
 * Test qui laisse évoluer l'environnement avec 20 balles 
 * et pendant 30 secondes
 *
 */

#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);
	GestionEnv_creerBalles(20);

/* DEBUT TEST */

	for(float i = 0; i < 30; i+=uniteTemps){
		GestionEnv_evoluer();
	}

/* FIN TEST */

	pause();

	GestionEnv_supprimerBalles();

	GestionEnv_supprimerPanier();
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
