/**
 * @file testBalles.c
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

	Environnement* envJeu = GestionEnv_creerEnvironnement();

/* DEBUT TEST */

	for(float i = 0; i < 30; i+=uniteTemps){
		GestionEnv_evoluer(envJeu);
	}

/* FIN TEST */

	pause();
	GestionEnv_supprimerEnvironnement(envJeu);

	return 0;
}
