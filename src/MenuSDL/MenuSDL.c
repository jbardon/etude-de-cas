/**
 * @file MenuSDL.c
 * @brief Affichage d'un menu pour les scores au algorithmes
 *
 * Construction automatique d'une surface SDL à partir 
 * de données passés en paramètre
 *
 * Utilisé pour afficher les lettres sélectionnée et les scores
 * aux 3 algorithmes de recherche de mots
 *
 */

#include "MenuSDL.h"

/** 
	@brief Centrer le message sur la surface
	@see _afficherMessage
 */
#define CENTRER -1

//-------------------------------------------------------------------------------------------------------------
//										Déclaration des fonctions locales
//-------------------------------------------------------------------------------------------------------------

static char* _Solution_afficherScore(Solution* motTrouve);
static void _afficherMessage(SDL_Surface* ecran, char* message, int x, int y, TTF_Font* police);
static void _afficherSolutions(SDL_Surface* ecran, Solution** motsTrouves, int nbSolutions);

//-------------------------------------------------------------------------------------------------------------
//											Gestion des solutions
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn Solution* Solution_creer(char* mot, int score)
 * @brief Création d'une solution
 *
 * @param mot Mot trouvé par un algorithme de recherche
 * @param score Score lié au mot trouvé (sera affiché à côté)
 * 
 * @return La solution contenant le mot et le score
 *
 * @see _afficherSolutions
 * @see Solution_supprimer
 */
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

/**
 * @fn void Solution_supprimer(Solution* motTrouve)
 * @brief Suppression d'une solution
 *
 * @param motTrouve Solution à supprimer
 *
 * @see Solution_creer
 */
void Solution_supprimer(Solution* motTrouve){
	free(motTrouve->mot);
	free(motTrouve);
}

/**
 * @fn static char* _Solution_afficherScore(Solution* motTrouve)
 * @brief Retourne le score sous forme d'une chaine de caractères
 *
 * @param motTrouve Solution contenant le score à traiter
 * @return Score sous forme de chaine de caractères
 */
static char* _Solution_afficherScore(Solution* motTrouve){
	static char buffer[7] = { 0 }; // motTrouve->score est un int 
	sprintf(buffer, "%d", motTrouve->score);
	return buffer;
}
//-------------------------------------------------------------------------------------------------------------
//												Menu et affichage
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn SDL_Surface* MenuSDL_creer(SDL_Surface* ecran, char* lettres, Solution** motsTrouves, int nbSolutions)
 * @brief Création d'un menu à partir des résultats donnés
 *
 * @param lettres Lettres sélectionnés avec la fonction GestionEnv_donnerCaracteresLigne
 * @param motsTrouves Solutions trouvées par les algorithmes de recherche pour les lettres données
 * @param nbSolutions Taille du tableau motsTrouves
 *
 * @return Menu créé à partir des données
 *
 * @see GestionEnv_donnerCaracteresLigne
 */
SDL_Surface* MenuSDL_creer(char* lettres, Solution** motsTrouves, int nbSolutions){

	// Affiche le fond du menu
	SDL_Surface* menu = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGUEUR_ECRAN, 100, 32, 0, 0, 0, 0);

	SDL_FillRect(menu, NULL, 0xA9EAFE);
	SDL_Rect zoneSolutions = { 0, 65, LARGUEUR_ECRAN, 35 };
	SDL_FillRect(menu, &zoneSolutions, 0x74D0F1);

	// Affiche les lettres
	char* message = calloc(strlen(lettres) + strlen("Lettres selectionnees:  \\") + 2 + 1, sizeof(char));
	sprintf(message, "Lettres selectionnees: %s \\%d", lettres, strlen(lettres));
	_afficherMessage(menu, message, CENTRER, 20, NULL);		
	free(message);	

	// Affiche les scores
	_afficherSolutions(menu, motsTrouves, nbSolutions);

	return menu;
}

/**
 * @fn static void _afficherSolutions(SDL_Surface* ecran, Solution** motsTrouves, int nbSolutions)
 * @brief Affiche toutes les solutions sur une surface SDL
 *
 * @param ecran Surface sur laquelle afficher les solutions
 * @param motsTrouves Solutions à afficher
 * @param nbSolutions Taille du tableau motsTrouves
 */
static void _afficherSolutions(SDL_Surface* ecran, Solution** motsTrouves, int nbSolutions){
	
	// Création des polices pour les mots et les scores	
	TTF_Font* policeMot = TTF_OpenFont("arial.ttf", 15);

	TTF_Font* policeSolution = TTF_OpenFont("arial.ttf", 18);
	TTF_SetFontStyle(policeSolution, TTF_STYLE_BOLD);

	int marge = 10;
	int w = 0;
	int x[nbSolutions][2];
	
	// Calcul la position en x du premier mots et du premier score
	x[0][0] = 0;
	TTF_SizeText(policeMot, motsTrouves[0]->mot, &w, NULL);
	x[0][1] = w + marge;

	// Calcul la position en x des mots et des scores
	for(int i = 1; i < nbSolutions; i++){
		TTF_SizeText(policeSolution, _Solution_afficherScore(motsTrouves[i-1]), &w, NULL);
		x[i][0] = x[i-1][1] + w + marge;
		TTF_SizeText(policeMot, motsTrouves[i]->mot, &w, NULL);
		x[i][1] = x[i][0] + w + marge;
	}	

	// Calcul le décalage pour tout centrer 
	TTF_SizeText(policeMot, _Solution_afficherScore(motsTrouves[nbSolutions-1]), &w, NULL);
	w += x[nbSolutions-1][1] + marge;
	w = (LARGUEUR_ECRAN - w)/2;

	// Affiche les mots et les scores
	for(int i = 0; i < nbSolutions; i++){
		_afficherMessage(ecran, motsTrouves[i]->mot, x[i][0] + w, 75, policeMot);
		_afficherMessage(ecran, _Solution_afficherScore(motsTrouves[i]), x[i][1] + w, 72, policeSolution);
	}

	// Supprime les polices
	TTF_CloseFont(policeMot);
	TTF_CloseFont(policeSolution);
}

/**
 * @fn static void _afficherMessage(SDL_Surface* ecran, char* message, int x, int y, TTF_Font* police)
 * @brief Affiche un message sur une surface SDL
 *
 * @param ecran Surface sur laquelle afficher le message
 * @param message Message à afficher
 * @param x Position en abscisse à laquelle afficher
 * @param y Position en ordonnée à laquelle afficher
 * @param police Police de caractères à utiliser
 *
 * @warning Donner CENTRER pour x permet de centrer le message horizontalement
 */
static void _afficherMessage(SDL_Surface* ecran, char* message, int x, int y, TTF_Font* police){

	SDL_Color couleur = {20, 50, 50};
	
	// Charge la police
	int libererPolice = 0;
	if(police == NULL){
		police = TTF_OpenFont("arial.ttf", 20);
		TTF_SetFontStyle(police, TTF_STYLE_BOLD);
		libererPolice = 1;
	}

	// Calcul la position
	if(x == CENTRER){
		int w = 0;
		TTF_SizeText(police, message, &w, NULL);
		x = (LARGUEUR_ECRAN - w)/2;
	}

	SDL_Surface* texte = TTF_RenderText_Blended(police, message, couleur);
	SDL_Rect position = {x, y};

	// Affiche le message
	SDL_BlitSurface(texte, NULL, ecran, &position);

	// Libération de la mémoire
	if(libererPolice){
		TTF_CloseFont(police);
	}
	SDL_FreeSurface(texte);

	SDL_Flip(ecran);
}


