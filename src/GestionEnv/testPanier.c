#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);

	/* DEBUT DEBUG */
	lineColor(ecran, 0, HAUTEUR_ECRAN - OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN - OFFSET, 0x00FFFF);
	lineColor(ecran, OFFSET, 0, OFFSET, HAUTEUR_ECRAN, 0x00FFFF);
	lineColor(ecran, LARGUEUR_ECRAN - OFFSET, 0, LARGUEUR_ECRAN - OFFSET, HAUTEUR_ECRAN, 0x00FFFF);
	/* FIN DEBUG */

	SDL_Flip(ecran);

	pause();

	GestionEnv_supprimerPanier();
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
