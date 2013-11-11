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

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);

/* DEBUT TEST */

	lineColor(ecran, 0, HAUTEUR_ECRAN - OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN - OFFSET, 0x00FFFF);
	lineColor(ecran, OFFSET, 0, OFFSET, HAUTEUR_ECRAN, 0x00FFFF);
	lineColor(ecran, LARGUEUR_ECRAN - OFFSET, 0, LARGUEUR_ECRAN - OFFSET, HAUTEUR_ECRAN, 0x00FFFF);

/* FIN TEST */

	SDL_Flip(ecran);

	pause();

	GestionEnv_supprimerPanier();
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
