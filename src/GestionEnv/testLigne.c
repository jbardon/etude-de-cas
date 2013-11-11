/**
 * @file testLigne.c
 * @brief Fait evoluer un environnement avec la bibliothèque
 * GestionEnv et trace une ligne pour récupérer les lettres 
 * des balles traversées
 * @author Jérémy.B
 * @version 1.0
 *
 * Test pour les fonctions:
 *   @see GestionEnv_donnerCaracteresLigne
 *   @see GestionEnv_ballesImmobiles
 *
 * Trace une ligne est vérifie quelles balles sont traversées
 * Les lettres des balles traversées sont affichées dans le terminal
 *
 * La boucle d'évilution de l'environnement s'arrête lorsque 
 * toutes les balles sont immobiles
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

	do {		
		GestionEnv_evoluer();
	}
	while(!GestionEnv_ballesImmobiles());

	int x1 = OFFSET + 100, y1 = HAUTEUR_ECRAN - OFFSET - 150;
	int x2 = LARGUEUR_ECRAN - OFFSET - 100, y2 = HAUTEUR_ECRAN - OFFSET - 50;
	char* lettres = GestionEnv_donnerCaracteresLigne(x1, y1, x2, y2);

	printf("Lettres sélectionnées: %s\n", lettres);

/* FIN TEST */

	pause();

	GestionEnv_supprimerBalles();
	GestionEnv_supprimerPanier();

	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
