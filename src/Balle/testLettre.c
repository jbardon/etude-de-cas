/**
 * @file testLettre.c
 * @brief Affichage de tout les caractères un à un sur une Balle
 * @author Jérémy.B
 * @version 1.0
 *
 * Test pour la bibliothèque Balle pour corriger les bugs suivants
 * 	  - la première balle affiche mal son caractère
 *	  - certains caractères ne sont pas au centre de la balle
 *
 * Voir la section bug connus pour trouver la solution
 *
 * Pour faire avancer le test, appuyer sur une touche du clavier 
 * dans le terminal.
 *
 */

#include <unistd.h> //pause
#include <Balle.h>

int main(void){

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);

	cpSpace* espace = cpSpaceNew();
	cpVect gravite = cpv(0, -100);  
	cpSpaceSetGravity(espace, gravite);

/* DEBUT TEST */

	TTF_Init();
	for(char l = 65; l < 91; l++){
		Balle* balle = Balle_creer(ecran, espace, cpv(100,100), cpvzero, 50, 0x00FF00FF, l);
		SDL_Flip(ecran);
		getchar();	
		Balle_supprimer(balle);
	}
	TTF_Quit();

/* FIN TEST */

	cpSpaceFree(espace);
	SDL_FreeSurface(ecran);
	SDL_Quit();

	return 0;
}
