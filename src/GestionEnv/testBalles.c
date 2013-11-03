#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);
	GestionEnv_creerBalles(20);

	for(float i = 0; i < 15; i+=uniteTemps){
		GestionEnv_evoluer();
	}

	pause();

	GestionEnv_supprimerBalles();

	GestionEnv_supprimerPanier();
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
