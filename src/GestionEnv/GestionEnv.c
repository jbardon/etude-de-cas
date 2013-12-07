/**
 * @file GestionEnv.c
 * @brief Gestion de l'environnement graphique et physique du jeu
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Bibliothèque pour la gestion de la SDL, chipmunk, des balles et
 * du tracé des lignes pour sélectionner des balles
 *
 */

#include "GestionEnv.h"

//-------------------------------------------------------------------------------------------------------------
//										Déclaration des fonctions locales
//-------------------------------------------------------------------------------------------------------------

/* Environnement physique et graphique */
static cpSpace* _initChipmunk();
static SDL_Surface* _initSDL();

static void _quitChipmunk(cpSpace* espace);
static void _quitSDL(SDL_Surface* ecran);

/* Panier */
static void _creerPanier(Environnement* envJeu);
static void _supprimerPanier(cpShape** panier);
static void _dessinerPanier();

/* Balles */
static void _creerBalles(Environnement* envJeu, int nbBalles);
static void _creerUneBalle(Environnement* envJeu);
static void _supprimerBalles(GPtrArray* balles);

static void _Balle_foreach(GPtrArray* balles, Balle_Fonction fonction);
static void _afficherProgression(Environnement* envJeu);

//-------------------------------------------------------------------------------------------------------------
//										Initialisation de l'environnement
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn Environnement* GestionEnv_creerEnvironnement()
 * @brief Créé un nouvel environnement
 * 
 * @see _initChipmunk
 * @see _initSDL
 * @see _creerPanier
 * @see _creerBalles
 *
 * @return L'environnement créé
 */
Environnement* GestionEnv_creerEnvironnement(){
	
	Environnement* envJeu = malloc(sizeof(Environnement));

	if(envJeu){

		Aleatoire_init();

		envJeu->espace = _initChipmunk();
		envJeu->ecran = _initSDL();
		envJeu->balles = NULL;
		envJeu->nbBallesTotal = 0;
		envJeu->nbBallesCrees = 0;
		envJeu->ajouterBalles = 0;

		_creerPanier(envJeu);
		_creerBalles(envJeu, NB_BALLES);
	}

	return envJeu;
}

/**
 * @fn void GestionEnv_rejouer(Environnement* envJeu)
 * @brief Paramètre l'environnement pour rejouer une partie
 *
 * @param envJeu Environnement de jeu
 */
void GestionEnv_rejouer(Environnement* envJeu){
	_supprimerBalles(envJeu->balles);
	envJeu->nbBallesTotal = 0;
	envJeu->nbBallesCrees = 0;
	_creerBalles(envJeu, NB_BALLES);
}

/**
 * @fn cpSpace* _initChipmunk()
 * @brief Fonction qui initialise le monde chipmunk
 * @return L'espace physique créé
 */
static cpSpace* _initChipmunk(){

	cpSpace* espace = cpSpaceNew();

	if(espace){
		cpVect gravite = cpv(0, -100);  
		cpSpaceSetGravity(espace, gravite);
	}

	return espace;
}

/**
 * @fn SDL_Surface* _initSDL()
 * @brief Fonction qui initialise la bibliothèque SDL et la fenêtre de jeu SDL
 * @return La fenêtre SDL
 */
static SDL_Surface* _initSDL(){

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);

	if(ecran){
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
		SDL_WM_SetCaption("Petanque 2000", NULL);
	}

	return ecran;	
}

//-------------------------------------------------------------------------------------------------------------
//										Suppression de l'environnement
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void GestionEnv_supprimerEnvironnement(Environnement* envJeu)
 * @brief Supprime un environnement de jeu
 * 
 * @param envJeu Environnement de jeu créé avec GestionEnv_creerEnvironnement
 *
 * @see GestionEnv_creerEnvironnement
 */
void GestionEnv_supprimerEnvironnement(Environnement* envJeu){
	_supprimerBalles(envJeu->balles);
	_supprimerPanier(envJeu->panier);

	_quitChipmunk(envJeu->espace);
	_quitSDL(envJeu->ecran);

	free(envJeu);
}

/**
 * @fn void _quitChipmunk(cpSpace* espace)
 * @brief Fonction qui détruit le monde chipmunk
 * 
 * @see _initChipmunk
 * @see espace
 */
static void _quitChipmunk(cpSpace* espace){
	cpSpaceFree(espace);
}

/**
 * @fn Svoid _quitSDL(SDL_Surface* ecran)
 * @brief Fonction qui détruit la fenêtre de jeu SDL
 * et quitte la bibliothèque SDL 
 * 
 * @see _initSDL
 * @see ecran
 */
static void _quitSDL(SDL_Surface* ecran){
	SDL_FreeSurface(ecran);
	TTF_Quit();
	SDL_Quit();
}

//-------------------------------------------------------------------------------------------------------------
//									Evolution de l'environnement (dans le temps)
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void GestionEnv_evoluer()
 * @brief Fonction qui fait evoluer l'environnement
 * physique et graphique du jeu avec notamment les balles 
 * qui se déplacent
 */
void GestionEnv_evoluer(Environnement* envJeu){
	
	static double temps = 0; // Temps qui s'écoule dans l'espace chipmunk
	static int timerLancement = 0; // Compteur de temps pour lancer les balles

	// Créé une balle à chaque intervalle de temps, max = nombre de balles donnée dans GestionEnv_creerBalles
	if(envJeu->ajouterBalles && timerLancement >= DELAI_APPARITION){
		if(envJeu->nbBallesCrees < envJeu->nbBallesTotal){
			_creerUneBalle(envJeu);
		}
		else {
			envJeu->ajouterBalles = 0;
		}
		timerLancement = 0;	
	}
/*
	// Met à jour l'affichage du panneau de la fenêtre
	char* message = calloc(strlen("Projet etude de cas - Balles 100") + 1, sizeof(char));
	sprintf(message, "Projet etude de cas - Balles %d", nbBallesCrees);
	SDL_WM_SetCaption(message, NULL);
	free(message);
*/
	// Fait évoluer les balles, applique le déplacement et maj de l'affichage
	cpSpaceStep(envJeu->espace, uniteTemps);

	_Balle_foreach(envJeu->balles, Balle_deplacer); 
	_dessinerPanier(envJeu->ecran); 
	_afficherProgression(envJeu);

	SDL_Flip(envJeu->ecran);

	// Gestion du temps
	temps += uniteTemps;
	timerLancement += 1;
}

/**
 * @fn static void _afficherProgression(Environnement* envJeu)
 * @brief Affiche la progression de l'ajout des balles
 *
 * Calcul le ration nbBallesCrees/nbBallesTotal et l'affiche
 * sous le panier sous la forme d'une ligne rouge
 *
 * @param envJeu Environnement de jeu dans lequel les balles ont été créées
 *
 * @warning La barre de progression ne s'affichera plus une fois toutes les balles créées
 */
static void _afficherProgression(Environnement* envJeu){	

	// Efface la barre de progression
	boxColor(envJeu->ecran, 0, HAUTEUR_ECRAN - 5, LARGUEUR_ECRAN, HAUTEUR_ECRAN, COULEUR_FOND);

	// Calcul et affiche la progression
	if(envJeu->ajouterBalles){
		float longueur = ((float)envJeu->nbBallesCrees/(float)envJeu->nbBallesTotal) * LARGUEUR_ECRAN;
		boxColor(envJeu->ecran, 0, HAUTEUR_ECRAN - 5, ceil(longueur), HAUTEUR_ECRAN, 0xFF4040DD);
	}
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion du panier
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void _creerPanier(cpSpace* espace, SDL_Surface* surf)
 * @brief Fonction qui créé le panier au sens physique (chipmunk)
 * et affiche ces contours sur la fenêtre SDL
 *
 * @param espace Espace physique chipmunk @see espace
 * @param surf Ecran principal de la fenêtre SDL @see ecran
 */
static void _creerPanier(Environnement* envJeu){
 
	int x = OFFSET + 2;
	int y = OFFSET;

	// Création du sol qui est un élément statique
	envJeu->panier[0] = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(envJeu->panier[0], FRICTION);
	cpShapeSetElasticity(envJeu->panier[0], REBOND);
	cpSpaceAddShape(envJeu->espace, envJeu->panier[0]);

	// Création du mur gauche
	envJeu->panier[1] = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(x,0), 
											cpv(x, HAUTEUR_ECRAN + BALLE_RAYON_MAX * 2), 0);

	cpShapeSetFriction(envJeu->panier[1], FRICTION);
	cpShapeSetElasticity(envJeu->panier[1], REBOND);
	cpSpaceAddShape(envJeu->espace, envJeu->panier[1]);

	// Création du mur droit
	x = LARGUEUR_ECRAN - OFFSET - 2;
	envJeu->panier[2] = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(x,0), 
											cpv(x, HAUTEUR_ECRAN + BALLE_RAYON_MAX * 2), 0);

	cpShapeSetFriction(envJeu->panier[2], FRICTION);
	cpShapeSetElasticity(envJeu->panier[2], REBOND);
	cpSpaceAddShape(envJeu->espace, envJeu->panier[2]);

	// Dessine le panier sur l'écran
	_dessinerPanier(envJeu->ecran);
}

/**
 * @fn static void _dessinerPanier(SDL_Surface* ecran)
 * @brief Dessine le panier sur une surface SDL
 *
 * @param ecran Ecran de jeu SDL
 */
static void _dessinerPanier(SDL_Surface* ecran){

	static const unsigned int e = 10; // Epaisseur du trait (SDL)
	static unsigned int x;            // Position en abscisse
	static unsigned int y;            // Position en ordonnée
	static unsigned int l;            // Longueur
	static unsigned int h;            // Hauteur

	// Initialisations
	x = OFFSET;
	l = LARGUEUR_ECRAN - 2*OFFSET;

	// Affichage du sol
	y = HAUTEUR_ECRAN - OFFSET + e/2;
	thickLineColor(ecran, x - e, y, x + l + e, y, e, 0x000000FF);

	// Affichage du mur gauche
	x = OFFSET - e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 3 * OFFSET;
	thickLineColor(ecran, x, y + e, x, h, e, 0x00000FF);

	// Affichage du mur droit
	x = LARGUEUR_ECRAN - OFFSET + e/2;
	thickLineColor(ecran, x, y + e, x, h, e, 0x000000FF);
}

/**
 * @fn void _supprimerPanier(cpShape** panier)
 * @brief Supprime le panier au sens physique (chipmunk)
 */
static void _supprimerPanier(cpShape** panier){
	for(int i = 0; i < 3; i++){
		cpShapeFree(panier[i]);
	}
}

/**
 * @fn void GestionEnv_effacerPanier(SDL_Surface* ecran)
 * @brief Efface tout le panier (bords compris)
 */
void GestionEnv_effacerPanier(SDL_Surface* ecran){
	boxColor(ecran, 0, 2*OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN, COULEUR_FOND);
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion des balles
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn static void _Balle_foreach(GPtrArray* balles, Balle_Fonction fonction)
 * @brief Fonction qui applique une fonction sur toutes
 * les balles présentes dans l'environnement
 *
 * @param balles Tableau de balles sur lequel appliquer la fonction
 * @param fonction Fonction provenant de la bibliothèque Balle
 * ayant aucun retour et pour seul paramètre une balle
 */
static void _Balle_foreach(GPtrArray* balles, Balle_Fonction fonction){
	g_ptr_array_foreach(balles, (GFunc)fonction, NULL);
}

/**
 * @fn static void _creerUneBalle(Environnement* envJeu)
 * @brief Fonction qui créé une balle dans l'environnement 
 * avec des paramètres aléatoires :
 *
 * @see Balle_creer
 * rayon: 28 -> 36
 * centre: x (dans les panier), y (juste au dessus de la fenêtre)
 * direction: (0,-80) -> (0,80)
 * couleur: @see couleurs
 * lettre: A -> Z
 *
 * @param envJeu Environnement de jeu dans lequel créer la balle
 */
static void _creerUneBalle(Environnement* envJeu){

	// Caractéritiques aléatoires pour les balles
	const int rayon = Aleatoire_MinMax(BALLE_RAYON_MIN, BALLE_RAYON_MAX);
	const cpVect centre = Aleatoire_Position(rayon);
	const cpVect direction = Aleatoire_Direction();
	const Uint32 couleur = Aleatoire_Couleur();
	const char lettre = Aleatoire_Lettre();

	// Création de la balle
	g_ptr_array_add(envJeu->balles, Balle_creer(envJeu->ecran, envJeu->espace, centre, direction, rayon, couleur, lettre));
	envJeu->nbBallesCrees++;		
}

/**
 * @fn static void _creerBalles(Environnement* envJeu, int nbBalles)
 * @brief Fonction qui demande à l'environnement de créé n balles.
 *
 * A l'appel de cette fonction une seule balle est réélement créée (@see nbBallesCrees)
 * Les suivantes sont créés toutes les n secondes (@see timerLancement, 
 * @see DELAI_APPARITION dans config.h) par la fonction @see GestionEnv_evoluer
 *
 * @param envJeu Environnement de jeu dans lequel créer les balles
 * @param nbBalles Nombre de balles à créer
 *
 * @warning Les balles ne seront pas créées en même temps, voir GestionEnv_evoluer
 * @see GestionEnv_evoluer
 */
static void _creerBalles(Environnement* envJeu, int nbBalles){

	envJeu->balles = g_ptr_array_sized_new(nbBalles);
	envJeu->ajouterBalles = 1;

	if(envJeu->balles){
		envJeu->nbBallesTotal = nbBalles;	
		envJeu->nbBallesCrees = 0;	
		_creerUneBalle(envJeu);		
	}
}

/**
 * @fn static void _supprimerBalles(GPtrArray* balles)
 * @brief Supprime toutes les balles présentes dans l'environnement
 * @param balles Tableau des balles à supprimer
 */
static void _supprimerBalles(GPtrArray* balles){

//	GestionEnv_effacerPanier(); /* VITESSE */
//	GestionEnv_viderZoneMessage(); /* VITESSE */
	_Balle_foreach(balles, Balle_supprimer);
	 g_ptr_array_free(balles, 0);
}

/**
 * @fn int GestionEnv_ballesImmobiles(Environnement* envJeu)
 * @brief Retourne 1 si toutes les balles de l'environnement sont immobiles, 0 sinon
 *
 * @param envJeu Environnement de jeu dans lequel sont les balles
 * @see Balle_estImmobile
 */
int GestionEnv_ballesImmobiles(Environnement* envJeu){

	int ballesImmobiles = 1, i = 0;

	while(ballesImmobiles && i < envJeu->nbBallesCrees){
		ballesImmobiles = Balle_estImmobile(g_ptr_array_index(envJeu->balles, i++));
	}	

	return ballesImmobiles;	
}

//-------------------------------------------------------------------------------------------------------------
//								Gestion tracer ligne & récupération des caractères
//-------------------------------------------------------------------------------------------------------------

/**
 * @brief Fonction générique pour comparer 2 éléments de même type
 *
 * @see GestionEnv_donnerCaracteresLigne
 */
typedef int (*FonctionComparer)(const void *, const void *);

/** @brief Ligne tracée de la droite vers la gauche */
#define DIR_GAUCHE 1

/** @brief Ligne tracée de la gauche vers la droite */
#define DIR_DROITE -1

/**
 * @var static int sensLigne
 * @brief Sens dans lequel la ligne est tracée
 *
 * @see GestionEnv_donnerCaracteresLigne
 * @see _comparerBalles
 */ 
static int sensLigne = DIR_GAUCHE;

/**
 * @fn static int _comparerBalles(const Balle** a, const Balle** b)
 * @brief Compare 2 balles selon leur position en absisse
 *
 * @param a Balle n°1
 * @param b Balle n°2
 *
 * @return Retourne a->x - b->x ou b->x - a->x selon le sens de la ligne

 * @warning Prend en compte le sens de lequel la ligne a été tracée
 * @see sensLigne
 * @see GestionEnv_donnerCaracteresLigne
 */
static int _comparerBalles(const Balle** a, const Balle** b){
	return ((sensLigne == DIR_DROITE) ?  (*a)->cx -  (*b)->cx :  (*b)->cx -  (*a)->cx);
}

/**
 * @fn char* GestionEnv_donnerCaracteresLigne(Environnement* envJeu, int x1, int y1, int x2, int y2)
 * @brief Fonction qui affiche une ligne de (x1,y1) à (x2,y2) et renvoie
 * la chaine de caractères formée par les lettres des balles traversés
 * par cette ligne
 * 
 * @param envJeu Environnement de jeu dans lequel tracer la ligne
 * @param x1 Abscisse du point de départ de la ligne
 * @param y1 Ordonnée du point de départ de la ligne
 * @param x2 Abscisse du point final de la ligne
 * @param y2 Ordonnée du point final de la ligne
 *
 * @return Chaîne de caractère formée par les lettres des balles 
 * traversés par la ligne
 */
char* GestionEnv_donnerCaracteresLigne(Environnement* envJeu, int x1, int y1, int x2, int y2){

	// Dessine la ligne
	lineColor(envJeu->ecran, x1, y1, x2, y2, 0x000000FF);

	// Cherche les balles touchées
	const unsigned int nbBallesMaxDiag = ceil(sqrt(pow(HAUTEUR_ECRAN,2) + 
										      pow(LARGUEUR_ECRAN,2))/(2*BALLE_RAYON_MAX)) * 2;

	unsigned int nbBallesTouches = 0;	
	Balle** ballesTouches = calloc(nbBallesMaxDiag, sizeof(Balle*));	

	for(unsigned int i = 0; i < envJeu->nbBallesCrees; i++){
		
		// Vérifie si la balle est touchée par la ligne
		int touche =  cpShapeSegmentQuery(
						   ((Balle*)g_ptr_array_index(envJeu->balles, i))->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		// Sauvegarde la balle touchée
		if(touche){
			ballesTouches[nbBallesTouches] = (Balle*)g_ptr_array_index(envJeu->balles, i);
			nbBallesTouches++;
		}
	}

	// Met les balles dans l'ordre suivant le tracé de la ligne
	// pour avoir les caractères dans le bon ordre (pour version 1 & 2 de l'algo de recherche)
	sensLigne = ((x1 < x2) ? DIR_DROITE : DIR_GAUCHE);
	qsort(ballesTouches, nbBallesTouches, sizeof(Balle*), (FonctionComparer) _comparerBalles);

	// Construit la chaine de caractères
	char* lettres = (char*) calloc(nbBallesTouches + 1, sizeof(char));

	for(unsigned int i = 0; i < nbBallesTouches; i++){
		char l [2] = { ballesTouches[i]->lettre, '\0' }; // Ajoute zéro terminal
		strcat(lettres, l);
	}

	// Supprime les balles touchées
	envJeu->nbBallesCrees -= nbBallesTouches;
	for(unsigned int i = 0; i < nbBallesTouches; i++){
		g_ptr_array_remove(envJeu->balles, ballesTouches[i]);
		Balle_supprimer(ballesTouches[i]);	
	}
	free(ballesTouches);

	// Met à jour l'affichage
	SDL_Flip(envJeu->ecran);

	return lettres;
}

//-------------------------------------------------------------------------------------------------------------
//								Gestion de l'affichage et de l'effacement
//-------------------------------------------------------------------------------------------------------------

/**
 * @var static int nbMessages
 * @brief Nombre de message écrit dans la zone des message
 *
 * Est incrémenté par GestionEnv_afficherMessage et remise zéro 
 * par GestionEnv_viderZoneMessage
 *
 * Permet de n'afficher le fond de la zone des message qu'une fois
 * pour éviter de masquer les anciens messages
 */ 
static int nbMessages = 0; 

/**
 * @fn void GestionEnv_afficherMessage(Environnement* envJeu, char* message, int x, int y, int taille)
 * @brief Affiche un message an haut de l'écran dans la zone dédiée
 *
 * Le premier message affiché après appel à GestionEnv_viderZoneMessage
 * applique un fond à la zone d'affichage
 *
 * @param envJeu Environnement de jeu dans lequel afficher le message
 * @param message Message à afficher
 * @param x Position en abscisse ou afficher le message
 * @param y Position en ordonnée ou afficher le message
 * @param taille Taille de la police
 *
 * @warning Cette fonction ne prend pas en compte la collision entre 2 messages
 * écrit au même endroit, veuillez utiliser GestionEnv_viderZoneMessage	
 */
void GestionEnv_afficherMessage(Environnement* envJeu, char* message, int x, int y, int taille){

	if(!nbMessages++){
		boxColor(envJeu->ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, 0xA9EAFEFF);
	}

	// Chargement de la police
	SDL_Color couleur = {20, 50, 50};
	
	TTF_Font* police = TTF_OpenFont("arial.ttf", taille);
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);

	// Calcul de la position
	if(x == ALIGN_CENTRE){
		int w = 0;
		TTF_SizeText(police, message, &w, NULL);
		x = (LARGUEUR_ECRAN - w)/2;
	}

	SDL_Surface* texte = TTF_RenderText_Blended(police, message, couleur);
	SDL_Rect position = {x, y};

	// Affichage
	SDL_BlitSurface(texte, NULL, envJeu->ecran, &position);
	SDL_FreeSurface(texte);
	TTF_CloseFont(police);	

	SDL_Flip(envJeu->ecran);

}

/**
 * @fn void GestionEnv_viderZoneMessage(Environnement* envJeu)
 * @brief Efface la zone des message en haut de l'écran
 * @param envJeu Environnement de jeu dans lequel est la zone des messages
 * @see GestionEnv_afficherMessage
 */
void GestionEnv_viderZoneMessage(Environnement* envJeu){
	nbMessages = 0;
	boxColor(envJeu->ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, COULEUR_FOND);
	SDL_Flip(envJeu->ecran);
}

//-------------------------------------------------------------------------------------------------------------
//						Fonctions debug (laisser pour compiler testDroiteGauche & testGaucheDroite)
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn cpShape** donnerSol()
 * @brief Fonction qui retourne le sol du panier
 * pour tester le comportement de balle avec chipmunk
 * 
 * @see testDroiteGauche
 * @see testGaucheDroite
 *
 * @return Pointeur sur le sol du panier
 */
cpShape** donnerSol(Environnement* envJeu){
	return envJeu->panier;
}

