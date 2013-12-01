#include <stdio.h>
#include <GestionEnv.h>
#include <AlgoRecherche.h>
#include <MenuSDL.h>

void attendreCommencer(){

    int continuer = 1;
    SDL_Event event;
 
    while (continuer){
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN
			&& event.key.keysym.sym == SDLK_SPACE){
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
	GestionEnv_effacerPanier(); /* A VOIR */
	GHashTable* dicoV1 = chargerDicoV1_V2("dicofinal.txt");
	GHashTable* dicoV3 = chargerDicoV3("dicofinal.txt");

	// Buffer pour afficher des messages sur la fenêtre
	char message[50];

	// Mot trouvés par les 3 versions
	Solution* motsTrouves[3] = { NULL };

	unsigned int rejouer = 1;
	while(rejouer){

		GestionEnv_creerBalles(55);

		// Attend que l'utilisateur appuis sur espace
		GestionEnv_viderZoneMessage();
		sprintf(message, "Appuyez sur [ESPACE] pour commencer");
		GestionEnv_afficherMessage(message, ALIGN_CENTRE, 35, 20);
		attendreCommencer();
		GestionEnv_viderZoneMessage();

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
		GestionEnv_afficherMessage(message, ALIGN_CENTRE, 35, 20);

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

		// Cherche un mot dans le dictionnaire
		for(int i = 0; i < strlen(lettres); i++){
			lettres[i] = tolower(lettres[i]);
		}
		
		int nbMotsTrouves = 0;
		char* motsVersions [3] = { NULL };

		motsVersions[nbMotsTrouves] = version1(dicoV1, lettres);
		if(motsVersions[nbMotsTrouves] != NULL){
			nbMotsTrouves++;
		}

		motsVersions[nbMotsTrouves] = version2(dicoV1, lettres);
		if(motsVersions[nbMotsTrouves] != NULL && strcmp(motsVersions[nbMotsTrouves], "") != 0){
			nbMotsTrouves++;
		}

		motsVersions[nbMotsTrouves] = version3(dicoV3, lettres);
		if(motsVersions[nbMotsTrouves] != NULL && strcmp(motsVersions[nbMotsTrouves], "") != 0){
			nbMotsTrouves++;
		}

		lettres = majuscules(lettres);

		if(nbMotsTrouves > 0){

			// Définition des couples mot-score
			for(int i = 0; i < nbMotsTrouves; i++){
				motsTrouves[i] = Solution_creer(majuscules(motsVersions[i]), strlen(motsVersions[i]));
			}
	
			SDL_Surface* menu = MenuSDL_creer(ecran, lettres, motsTrouves, nbMotsTrouves);
			SDL_Rect pos = { 0, 0 };
			SDL_BlitSurface(menu, NULL, ecran, &pos);
			SDL_Flip(ecran);

			// Supprime le menu et les couples mot-score
			for(int i = 0; i < nbMotsTrouves; i++){
				Solution_supprimer(motsTrouves[i]);
			}

			SDL_FreeSurface(menu);
		}
		else {
			GestionEnv_viderZoneMessage();
			sprintf(message, "Aucun mot trouve avec %s", lettres);
			GestionEnv_afficherMessage(message, ALIGN_CENTRE, 20, 20);			
		}

		// Supprime les balles sélectionneés
		GestionEnv_effacerPanier();
		do {		
			GestionEnv_evoluer();
		}
		while(!GestionEnv_ballesImmobiles());	

		// Demande de recommencer
		sprintf(message, "(Appuyez sur [Q] pour quitter ou [R] pour rejouer)");
		GestionEnv_afficherMessage(message, ALIGN_CENTRE, 45, 12);

		// Attent réponse pour rejouer ou quitter
		rejouer = attendreReponseQR();

		// Supprime toutes les balles
		GestionEnv_supprimerBalles();

		// Efface tout l'écran
		SDL_FillRect(ecran, NULL, COULEUR_FOND);

	}

	// Libération du dictionnaire
	g_hash_table_destroy(dicoV1);	
	g_hash_table_destroy(dicoV3); /* A VOIR */
	
	// Libération du panier
	GestionEnv_supprimerPanier();

	// Ferme la SDL et chipmunk
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return EXIT_SUCCESS;
}
