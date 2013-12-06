#include <stdio.h>
#include <sys/time.h>

#include <GestionEnv.h>
#include <AlgoRecherche.h>
#include <MenuSDL.h>
#include <unistd.h>

cpVect AttendreClic();
void attendreCommencer();
int attendreReponseQR();
char* majuscules(char* chaine);
char* minuscules(char* chaine);

int main(void){


	// Initialisation de la SDL et de chipmunk
	Environnement* envJeu = GestionEnv_creerEnvironnement();

	// Créé le panier et charge le dictionnaire
	//GestionEnv_creerPanier(espace, ecran);
	//GestionEnv_effacerPanier(); /* A VOIR */
	GHashTable* dicoV1 = chargerDicoV1_V2("dicofinal.txt");
	GHashTable* dicoV3 = chargerDicoV3("dicofinal.txt");

	// Buffer pour afficher des messages sur la fenêtre
	char message[80];

	// Mot trouvés par les 3 versions
	Solution* motsTrouves[3] = { NULL };

	// Score maximum
	char* scoreMax = NULL;

	// Mesure du temps d'execution des algos
	struct timeval debut, fin; 
	double duree = 0;

	unsigned int rejouer = 1;
	while(rejouer){

		// Attend que l'utilisateur appuis sur espace
		GestionEnv_viderZoneMessage(envJeu);
		GestionEnv_effacerPanier(envJeu->ecran);
		sprintf(message, "Appuyez sur [ESPACE] pour commencer");
		GestionEnv_afficherMessage(envJeu, message, ALIGN_CENTRE, 35, 20);
		attendreCommencer();
		GestionEnv_viderZoneMessage(envJeu);

		/*
			long int screen = 0;
			sprintf(message, "/home/jeremy/Images/projet/projet2/image_%04ld.bmp", screen);
			SDL_SaveBMP(ecran, message);
			screen++;
		*/

		// Fait évoluer le système tant que toutes les boules sont en mouvement
		do {	
			SDL_FillRect(envJeu->ecran, NULL, 0xFFFFFF);
			GestionEnv_evoluer(envJeu);
		}
		while(!GestionEnv_ballesImmobiles(envJeu));

		// Attent les 2 clic de l'utilisateur pour tracer la ligne
		GestionEnv_viderZoneMessage(envJeu);
		sprintf(message, "Cliquez dans la fenetre pour tracer une ligne");
		GestionEnv_afficherMessage(envJeu, message, ALIGN_CENTRE, 35, 20);

		// Récupère les coordonnées du clic et trace un point à cet emplacement
		cpVect coordonneesLigne[2];
		for(unsigned int i = 0; i < 2; i++){
			coordonneesLigne[i] = AttendreClic();
			filledCircleColor(envJeu->ecran, coordonneesLigne[i].x, coordonneesLigne[i].y, 4, 0x000000FF);
			SDL_Flip(envJeu->ecran);
		}

		// Récupère les caractères des balles sélectionnées par l'utilisateur
		char* lettres = GestionEnv_donnerCaracteresLigne(envJeu, coordonneesLigne[0].x, coordonneesLigne[0].y, 
														 coordonneesLigne[1].x, coordonneesLigne[1].y);
		printf("Lettres selectionnes: %s \\%d\n", lettres, strlen(lettres));

		// Cherche un mot dans le dictionnaire
		lettres = minuscules(lettres);
		
		int nbMotsTrouves = 0;
		char* motsVersions [3] = { NULL };

		gettimeofday(&debut, NULL);
		motsVersions[nbMotsTrouves] = version1(dicoV1, lettres);
		gettimeofday(&fin, NULL);
		if(strcmp(motsVersions[nbMotsTrouves], "") != 0){
			nbMotsTrouves++;
		}
		duree = ((double)(1000*(fin.tv_sec-debut.tv_sec)+((fin.tv_usec-debut.tv_usec)/1000)))/1000.;
		printf("Algo 1: %s trouve en %2.4fs\n", majuscules(motsVersions[0]), duree);

		gettimeofday(&debut, NULL);
		motsVersions[nbMotsTrouves] = version2(dicoV1, lettres);
		gettimeofday(&fin, NULL);
		if(strcmp(motsVersions[nbMotsTrouves], "") != 0){
			nbMotsTrouves++;
		}
		duree = ((double)(1000*(fin.tv_sec-debut.tv_sec)+((fin.tv_usec-debut.tv_usec)/1000)))/1000.;
		printf("Algo 2: %s trouve en %2.4fs\n", majuscules(motsVersions[1]), duree);

		gettimeofday(&debut, NULL);
		motsVersions[nbMotsTrouves] = version3(dicoV3, lettres);
		gettimeofday(&fin, NULL);
		if(strcmp(motsVersions[nbMotsTrouves], "") != 0){
			nbMotsTrouves++;
		}
		duree = ((double)(1000*(fin.tv_sec-debut.tv_sec)+((fin.tv_usec-debut.tv_usec)/1000)))/1000.;
		printf("Algo 3: %s trouve en %2.4fs\n", majuscules(motsVersions[2]), duree);

		lettres = majuscules(lettres);
		
		if(nbMotsTrouves > 0){

			// Définition des couples mot-score
			for(int i = 0; i < nbMotsTrouves; i++){
				motsTrouves[i] = Solution_creer(majuscules(motsVersions[i]), strlen(motsVersions[i]));
			}
	
			SDL_Surface* menu = MenuSDL_creer(envJeu->ecran, lettres, motsTrouves, nbMotsTrouves);
			SDL_Rect pos = { 0, 0 };
			SDL_BlitSurface(menu, NULL, envJeu->ecran, &pos);
			SDL_Flip(envJeu->ecran);

			// Supprime le menu et les couples mot-score
			for(int i = 0; i < nbMotsTrouves; i++){
				Solution_supprimer(motsTrouves[i]);
			}

			SDL_FreeSurface(menu);
		}
		else {
			GestionEnv_viderZoneMessage(envJeu);
			sprintf(message, "Aucun mot trouve pour: %s", lettres);
			GestionEnv_afficherMessage(envJeu, message, ALIGN_CENTRE, 20, 20);			
		}

		// Met à jour le score maximum
		if(scoreMax == NULL){	
			scoreMax = calloc(strlen(motsVersions[2]) + 1, sizeof(char));	
			strcpy(scoreMax, motsVersions[2]);				
		}
		else if(strlen(motsVersions[2]) > strlen(scoreMax)){
			scoreMax = realloc(scoreMax, (strlen(motsVersions[2]) + 1) * sizeof(char));	
			strcpy(scoreMax, motsVersions[2]);					
		}

		// Affiche le score max
		sprintf(message, "Petanque 2000 - Meilleur score: %d avec %s", strlen(scoreMax), scoreMax);
		SDL_WM_SetCaption(message, NULL);

		// Supprime les résultats
		free(lettres);
		for(unsigned int i = 0; i < 3; i++){
			free(motsVersions[i]);
		}

		// Supprime les balles sélectionneés
		GestionEnv_effacerPanier(envJeu->ecran);
		do {	
			GestionEnv_effacerPanier(envJeu->ecran);
			GestionEnv_evoluer(envJeu);
		}
		while(!GestionEnv_ballesImmobiles(envJeu));	

		// Demande de recommencer
		sprintf(message, "(Appuyez sur [Q] pour quitter ou [R] pour rejouer)");
		GestionEnv_afficherMessage(envJeu, message, ALIGN_CENTRE, 45, 12);

		// Attent réponse pour rejouer ou quitter
		rejouer = attendreReponseQR();
		if(rejouer){
			GestionEnv_rejouer(envJeu);
		}

		// Supprime toutes les balles
		//GestionEnv_supprimerBalles();

		// Efface tout l'écran
		SDL_FillRect(envJeu->ecran, NULL, COULEUR_FOND);
		printf("\n");
	}

	free(scoreMax);

	// Libération du dictionnaire
	g_hash_table_destroy(dicoV1);
	//supprimerDicoV3(dicoV3); /* A VOIR */

	// Libération du panier
	//GestionEnv_supprimerPanier();

	// Ferme la SDL et chipmunk
	//GestionEnv_quitChipmunk();
	//GestionEnv_quitSDL();
	GestionEnv_supprimerEnvironnement(envJeu);

	return EXIT_SUCCESS;
}

cpVect AttendreClic(){

    SDL_Event event;
	cpVect positionSouris = cpvzero;
	int continuer = 1;

    while(continuer){
		SDL_WaitEvent(&event);

        switch (event.type){
		    case SDL_MOUSEMOTION:
		        positionSouris.x = event.motion.x;
		        positionSouris.y = event.motion.y;
		    break;
		    case SDL_MOUSEBUTTONDOWN:
		        continuer = 0;
		    break;
        }
    }

	return positionSouris;
}

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

char* minuscules(char* chaine){
	for(unsigned int i = 0; i < strlen(chaine); i++){
		chaine[i] = (char) tolower(chaine[i]);
	}
	return chaine;
}

char* majuscules(char* chaine){
	for(unsigned int i = 0; i < strlen(chaine); i++){
		chaine[i] = (char) toupper(chaine[i]);
	}
	return chaine;
}
