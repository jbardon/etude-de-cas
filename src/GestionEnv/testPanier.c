/**
 * @file testPanier.c
 * @brief Affichage le panier sur une fenêtre SDL
 * @author Jérémy.B
 * @version 1.0
 *
 * Test pour les fonctions :
 *    @see GestionEnv_creerPanier 
 *    @see GestionEnv_supprimerPanier
 *    @see GestionEnv_initSDL
 *    @see GestionEnv_quitSDL
 *    @see GestionEnv_initChipmunk
 *    @see GestionEnv_quitChipmunk
 *
 * Affiche les lignes du panier avec une épaisseur de 1px pour vérifier
 * que l'épaisseur est bien gérée et les lignes tracées sont au bon endroit
 *
 */

#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

int main(void){

	Environnement* envJeu = GestionEnv_creerEnvironnement();

/* DEBUT TEST */

	lineColor(envJeu->ecran, 0, HAUTEUR_ECRAN - OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN - OFFSET, 0x00FFFF);
	lineColor(envJeu->ecran, OFFSET, 0, OFFSET, HAUTEUR_ECRAN, 0x00FFFF);
	lineColor(envJeu->ecran, LARGUEUR_ECRAN - OFFSET, 0, LARGUEUR_ECRAN - OFFSET, HAUTEUR_ECRAN, 0x00FFFF);

/* FIN TEST */

	SDL_Flip(envJeu->ecran);

	pause();
	GestionEnv_supprimerEnvironnement(envJeu);

	return 0;
}
