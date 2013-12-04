#include "MenuSDL.h"

#define CENTRER -1
static void _afficherMessage(SDL_Surface* ecran, char* message, int x, int y, TTF_Font* police);
static void _afficherSolutions(SDL_Surface* ecran, Solution** motsTrouves, int nb);

Solution* Solution_creer(char* mot, int score){
	Solution* motTrouve = malloc(sizeof(Solution));

	if(score){
		
		motTrouve->score = score;
		motTrouve->mot = calloc(strlen(mot) + 1, sizeof(char));
		if(motTrouve->mot){
			strcpy(motTrouve->mot, mot);
		}
		else {
			free(motTrouve);
		}
	}

	return motTrouve;
}

void Solution_supprimer(Solution* motTrouve){
	free(motTrouve->mot);
	free(motTrouve);
}

char* Solution_afficherSolution(Solution* motTrouve){
	static char buffer[7] = { 0 }; //motTrouve->score est un int 
	sprintf(buffer, "%d", motTrouve->score);
	return buffer;
}

SDL_Surface* MenuSDL_creer(SDL_Surface* ecran, char* lettres, Solution** motsTrouves, int nbSolutions){

	// Affiche le fond du menu
	SDL_Surface* menu = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGUEUR_ECRAN, 100, 32, 0, 0, 0, 0);

	SDL_FillRect(menu, NULL, 0xA9EAFE);
	SDL_Rect zoneSolutions = { 0, 65, LARGUEUR_ECRAN, 35 };
	SDL_FillRect(menu, &zoneSolutions, 0x74D0F1);

	// Affiche les lettres
	char* message = calloc(strlen(lettres) + strlen("Lettres selectionnees:  - ") + 2 + 1, sizeof(char));
	sprintf(message, "Lettres selectionnees: %s - %d", lettres, strlen(lettres));
	_afficherMessage(menu, message, CENTRER, 20, NULL);		
	free(message);	

	// Affiche les scores
	_afficherSolutions(menu, motsTrouves, nbSolutions);

	return menu;
}

static void _afficherSolutions(SDL_Surface* ecran, Solution** motsTrouves, int nb){
	
	// Création des polices pour les mots et les scores	
	TTF_Font* policeMot = TTF_OpenFont("arial.ttf", 15);

	TTF_Font* policeSolution = TTF_OpenFont("arial.ttf", 18);
	TTF_SetFontStyle(policeSolution, TTF_STYLE_BOLD);

	int marge = 10;
	int w = 0;
	int x[nb][2];
	
	// Calcul la position en x du premier mots et du premier score
	x[0][0] = 0;
	TTF_SizeText(policeMot, motsTrouves[0]->mot, &w, NULL);
	x[0][1] = w + marge;

	// Calcul la position en x des mots et des scores
	for(int i = 1; i < nb; i++){
		TTF_SizeText(policeSolution, Solution_afficherSolution(motsTrouves[i-1]), &w, NULL);
		x[i][0] = x[i-1][1] + w + marge;
		TTF_SizeText(policeMot, motsTrouves[i]->mot, &w, NULL);
		x[i][1] = x[i][0] + w + marge;
	}	

	// Calcul le décalage pour tout centrer 
	TTF_SizeText(policeMot, Solution_afficherSolution(motsTrouves[nb-1]), &w, NULL);
	w += x[nb-1][1] + marge;
	w = (LARGUEUR_ECRAN - w)/2;

	// Affiche les mots et les scores
	for(int i = 0; i < nb; i++){
		_afficherMessage(ecran, motsTrouves[i]->mot, x[i][0] + w, 75, policeMot);
		_afficherMessage(ecran, Solution_afficherSolution(motsTrouves[i]), x[i][1] + w, 72, policeSolution);
	}

	// Supprime les polices
	TTF_CloseFont(policeMot);
	TTF_CloseFont(policeSolution);
}

static void _afficherMessage(SDL_Surface* ecran, char* message, int x, int y, TTF_Font* police){

	SDL_Color couleur = {20, 50, 50};
	
	if(police == NULL){
		police = TTF_OpenFont("arial.ttf", 20);
		TTF_SetFontStyle(police, TTF_STYLE_BOLD);
	}

	if(x == CENTRER){
		int w = 0;
		TTF_SizeText(police, message, &w, NULL);
		x = (LARGUEUR_ECRAN - w)/2;
	}

	SDL_Surface* texte = TTF_RenderText_Blended(police, message, couleur);
	SDL_Rect position = {x, y};

	SDL_BlitSurface(texte, NULL, ecran, &position);

	SDL_Flip(ecran);
}
