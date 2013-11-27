#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>
#include <AlgoRecherche.h>

void attendreFermeture(){

    int continuer = 1;
    SDL_Event event;
 
    while (continuer){
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT){
	        continuer = 0;
        }
    }
}

int attendreReponseQR(){

	unsigned int rejouer = 0;

    int continuer = 1;
    SDL_Event event;
 
    while (continuer){

        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_r:
					rejouer = 1;
					continuer = 0;
				break;

		        case SDLK_q:
		            rejouer = 0;
					continuer = 0;
				break;

				default:
				break;
			}
        }
		else if(event.type == SDL_QUIT){
	        continuer = 0;
        }
    }

	return rejouer;
}

char* majuscules(char* chaine){
	for(unsigned int i = 0; i < strlen(chaine); i++){
		chaine[i] = (char) toupper(chaine[i]);
	}
	return chaine;
}

typedef struct
{
    char key[SDLK_LAST];
    int mousex,mousey;
    int mousexrel,mouseyrel;
    char mousebuttons[8];
    char quit;
} Input;

void MiseAJourEvenements(Input* in){

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type){
		    case SDL_MOUSEMOTION:
		        in->mousex = event.motion.x;
		        in->mousey = event.motion.y;
		        in->mousexrel = event.motion.xrel;
		        in->mouseyrel = event.motion.yrel;
		        break;
		    case SDL_MOUSEBUTTONDOWN:
		        in->mousebuttons[event.button.button] = 1;
		        break;
		    case SDL_MOUSEBUTTONUP:
		        in->mousebuttons[event.button.button] = 0;
		        break;
		    case SDL_QUIT:
		        in->quit = 1;
		        break;
		    default:
		        break;
        }
    }
}

int main(void){

	// Initialisation de la SDL et de chipmunk
	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	// Créé le panier et charge le dictionnaire
	GestionEnv_creerPanier(espace, ecran);
	GHashTable* dico = chargerDico("dicofinal.txt");

	// Buffer pour afficher des messages sur la fenêtre
	char message[50];

	unsigned int rejouer = 1;
	while(rejouer){

		GestionEnv_creerBalles(55);

		/*
			long int screen = 0;
			sprintf(message, "/home/jeremy/Images/projet/projet2/image_%04ld.bmp", screen);
			SDL_SaveBMP(ecran, message);
			screen++;
		*/

		// Fait évoluer le système tant que toutes les boules sont en mouvement
		do {	
			GestionEnv_evoluer();
		}
		while(!GestionEnv_ballesImmobiles());

		// Attent les 2 clic de l'utilisateur pour tracer la ligne
		GestionEnv_viderZoneMessage();
		sprintf(message, "Cliquez dans la fenetre pour tracer une ligne");
		GestionEnv_afficherMessage(message, 105, 35, 20);

		int nbClic = 0;
		cpVect coord[2]; // Les extrémités de la ligne
		Input in;
		memset(&in, 0, sizeof(in));

		while(nbClic < 2){
		    MiseAJourEvenements(&in);

		    if (in.mousebuttons[SDL_BUTTON_LEFT]){
			
				// Trace un point à l'emplacement du clic
				filledCircleColor(ecran, in.mousex, in.mousey, 4, 0x000000FF);
			
				// Enregistre le clic et la position du pointeur
		        in.mousebuttons[SDL_BUTTON_LEFT] = 0;
				coord[nbClic] = cpv(in.mousex, in.mousey);
				nbClic++;

				// Met à jour l'affichage
				SDL_Flip(ecran);			
		    }            
		}

		// Récupère les caractères des balles sélectionnées par l'utilisateur
		char* lettres = GestionEnv_donnerCaracteresLigne(coord[0].x, coord[0].y, coord[1].x, coord[1].y);

		GestionEnv_viderZoneMessage();
		sprintf(message, "Lettres selectionnees: %s", majuscules(lettres));
		GestionEnv_afficherMessage(message, OFFSET, 20, 20);

		// Cherche un mot dans le dictionnaire
		for(int i = 0; i < strlen(lettres); i++){
		  lettres[i] = tolower(lettres[i]);
		}

		char* result = version1(lettres, dico);
	
		if(result){
			sprintf(message, "Mot trouve ! %s (%d pts)", majuscules(result), strlen(result));
			GestionEnv_afficherMessage(message, OFFSET, 50, 20);
		}
		else {
			sprintf(message, "Aucun mot trouve =(");
			GestionEnv_afficherMessage(message, OFFSET, 50, 20);
		}

		// Supprime les balles sélectionneés
		GestionEnv_effacerPanier();
		do {		
			GestionEnv_evoluer();
		}
		while(!GestionEnv_ballesImmobiles());	

		// Demande de recommencer
		GestionEnv_viderZoneMessage();
		sprintf(message, "Appuyez sur [Q] pour quitter ou [R] pour rejouer");
		GestionEnv_afficherMessage(message, 80, 35, 20);

		// Attent réponse pour rejouer ou quitter
		rejouer = attendreReponseQR();

		// Supprime toutes les balles
		GestionEnv_supprimerBalles();

		// Efface tout l'écran
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
step++;
	}

	// Libération du dictionnaire
	g_hash_table_destroy(dico);	
	
	// Libération du panier
	GestionEnv_supprimerPanier();

	// Ferme la SDL et chipmunk
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return EXIT_SUCCESS;
}
