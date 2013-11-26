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
static void _dessinerPanier();

static void _Balle_foreach(Balle_Fonction fonction);
static void _creerUneBalle();

static unsigned int _randMinMax(int min, int max);
static Uint32 _randCouleur();
static cpVect _randDirection();
static char _randLettre();
static cpVect _randPosition(int rayon);

//-------------------------------------------------------------------------------------------------------------
//										Variables liées à l'environnement
//-------------------------------------------------------------------------------------------------------------

/**
 * @var cpSpace* espace
 * @brief Espace physique au sens de chipmunk
 *
 * Initialisation: @see GestionEnv_initChipmunk
 * Destruction: @see GestionEnv_quitChipmunk
 *
 */ 
static cpSpace* espace = NULL;

/**
 * @var cpShape* panier[3]
 * @brief Limitations physique du saladier ou tombent
 * les balles
 *
 * @see GestionEnv_creerPanier
 * @see GestionEnv_supprimerPanier
 *
 * Ce tableau contient dans l'ordre le sol et les 2 murs (gauche, droite) 
 *
 */ 
static cpShape* panier[3]; 

/**
 * @var double temps
 * @brief Temps dans le monde chipmunk
 *
 * Temps qui s'écoule dans le monde chipmunk à 
 * chaque appel de la fonction GestionEnv_evoluer
 *
 * @see GestionEnv_evoluer
 *
 */ 
static double temps = 0;

/**
 * @var SDL_Surface* ecran
 * @brief Ecran principal de fenêtre SDL
 *
 * @see GestionEnv_initSDL
 * @see GestionEnv_quitSDL
 *
 */ 
static SDL_Surface* ecran = NULL;

/**
 * @var GSList balles
 * @brief Tableau des balles présentent dans l'environnement
 * Balles créées avec la fonction _creerUneBalle
 *
 * @see GestionEnv_creerBalles
 * @see _creerUneBalle
 *
 *
 */ 
static GPtrArray* balles = NULL;

/**
 * @var int nbBallesTotal
 * @brief Taille du tableau de balles
 *
 * @see balles
 */ 
static int nbBallesTotal = 0;  

/**
 * @var int nbBallesCrees
 * @brief Nombre de balles réélement créées dans l'environnement
 *
 * @see GestionEnv_creerBalles
 */ 
static int nbBallesCrees = 0; 

/**
 * @var int timerLancement
 * @brief Compteur de temps pour lancer les balles
 *
 * Le temps évolue avec GestionEnv_evoluer à travers
 * la variable temps. Une fois le compteur à son maximum (DELAI_APPARITION)
 * une balle est créée et le timer est remis à zéro
 *
 * @see temps
 * @see GestionEnv_evoluer
 * @see DELAI_APPARITION (config.h)
 */ 
static int timerLancement = 0;

//-------------------------------------------------------------------------------------------------------------
//										Initialisation de l'environnement
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn cpSpace* GestionEnv_initChipmunk()
 * @brief Fonction qui initialise le monde chipmunk
 * 
 * @see espace
 *
 * @return L'espace physique créé
 */
cpSpace* GestionEnv_initChipmunk(){

	srand(time(NULL));	/* A VOIR */
	espace = cpSpaceNew();

	if(espace){
		cpVect gravite = cpv(0, -100);  
		cpSpaceSetGravity(espace, gravite);
	}

	return espace;
}

/**
 * @fn SDL_Surface* GestionEnv_initSDL()
 * @brief Fonction qui initialise la bibliothèque SDL 
 * et la fenêtre de jeu SDL
 * 
 * @see ecran
 *
 * @return La fenêtre SDL
 */
SDL_Surface* GestionEnv_initSDL(){

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);

	if(ecran){
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
		SDL_WM_SetCaption("Projet etude de cas", NULL);
	}

	return ecran;	
}

//-------------------------------------------------------------------------------------------------------------
//										Suppression de l'environnement
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void GestionEnv_quitChipmunk()
 * @brief Fonction qui détruit le monde chipmunk
 * 
 * @see GestionEnv_initChipmunk
 * @see espace
 */
void GestionEnv_quitChipmunk(){
	cpSpaceFree(espace);
}

/**
 * @fn Svoid GestionEnv_quitSDL()
 * @brief Fonction qui détruit la fenêtre de jeu SDL
 * et quitte la bibliothèque SDL 
 * 
 * @see GestionEnv_initSDL
 * @see ecran
 */
void GestionEnv_quitSDL(){
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
void GestionEnv_evoluer(){

	// Fait évoluer les balles, applique le déplacement et maj de l'affichage
	cpSpaceStep(espace, uniteTemps);	
	_Balle_foreach(Balle_deplacer); 
	_dessinerPanier(); 
	SDL_Flip(ecran);

	// Créé une balle à chaque intervalle de temps, max = nombre de balles donnée dans GestionEnv_creerBalles
	if(nbBallesCrees < nbBallesTotal && timerLancement == DELAI_APPARITION){
		_creerUneBalle();
		timerLancement = 0;	
	}

	// Gestion du temps
	temps += uniteTemps;
	timerLancement += 1;
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion du panier
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf)
 * @brief Fonction qui créé le panier au sens physique (chipmunk)
 * et affiche ces contours sur la fenêtre SDL
 *
 * @param espace Espace physique chipmunk @see espace
 * @param surf Ecran principal de la fenêtre SDL @see ecran
 */
void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf){
 
	int x = OFFSET + 2;
	int y = OFFSET;

cpBodySetMoment(espace->staticBody, INFINITY);
cpBodySetMass(espace->staticBody, INFINITY);
cpBodySetForce(espace->staticBody, cpv(1,1));

	// Création du sol qui est un élément statique
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], FRICTION);
	cpShapeSetElasticity(panier[0], REBOND);
	cpSpaceAddShape(espace, panier[0]);

	// Création du mur gauche
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1], FRICTION);
	cpShapeSetElasticity(panier[1], REBOND);
	cpSpaceAddShape(espace, panier[1]);

	// Création du mur droit
	x = LARGUEUR_ECRAN - OFFSET - 2;
	panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[2], FRICTION);
	cpShapeSetElasticity(panier[2], REBOND);
	cpSpaceAddShape(espace, panier[2]);

	// Dessine le panier sur l'écran
	_dessinerPanier();
}

static void _dessinerPanier(){

	static const unsigned int e = 10; // Epaisseur du trait (SDL)
	static unsigned int x;
	static unsigned int y;
	static unsigned int l;
	static unsigned int h;

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
 * @fn void GestionEnv_supprimerPanier()
 * @brief Fonction qui supprime le panier au sens physique (chipmunk)
 */
void GestionEnv_supprimerPanier(){
	for(int i = 0; i < 3; i++){
		cpShapeFree(panier[i]);
	}
}

void GestionEnv_effacerPanier(){
	boxColor(ecran, 0, 2*OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN - 2*OFFSET, COULEUR_FOND);
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion des balles
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn static void _Balle_foreach(Balle_Fonction fonction)
 * @brief Fonction qui applique une fonction sur toutes
 * les balles présentes dans l'environnement
 *
 * @param fonction Fonction provenant de la bibliothèque Balle
 * ayant aucun retour et pour seul paramètre une balle
 */
static void _Balle_foreach(Balle_Fonction fonction){
	/*for(unsigned int i = 0; i < nbBallesCrees; i++){
		fonction(g_ptr_array_index(balles, i));
	}*/
	g_ptr_array_foreach(balles, (GFunc)fonction, NULL);
}

/**
 * @fn static void _creerUneBalle()
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
 * @param fonction Fonction provenant de la bibliothèque Balle
 * ayant aucun retour et pour seul paramètre une balle
 */
static void _creerUneBalle(){

	if(nbBallesCrees < nbBallesTotal){

		// Caractéritiques aléatoires pour les balles
		const int rayon = _randMinMax(28,36);
		const cpVect centre = _randPosition(rayon);
		const cpVect direction = _randDirection();
		const Uint32 couleur = _randCouleur();
		const char lettre = _randLettre();

		// Création de la balle
		//balles[nbBallesCrees] = Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre);
		g_ptr_array_add(balles, Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre));
		nbBallesCrees++;
	}			
}

/**
 * @fn void GestionEnv_creerBalles(int nbBalles)
 * @brief Fonction qui demande à l'environnement de créé n balles.
 *
 * A l'appel de cette fonction une seule balle est réélement créée (@see nbBallesCrees)
 * Les suivantes sont créés toutes les n secondes (@see timerLancement, 
 * @see DELAI_APPARITION dans config.h) par la fonction @see GestionEnv_evoluer
 *
 * @param fonction Fonction provenant de la bibliothèque Balle
 * ayant aucun retour et pour seul paramètre une balle
 */
void GestionEnv_creerBalles(int nbBalles){

	//balles = calloc(nbBalles, sizeof(Balle*));
	balles = g_ptr_array_sized_new(nbBalles);

	if(balles){
		nbBallesTotal = nbBalles;	
		nbBallesCrees = 0;	
		_creerUneBalle();		
	}
}

/**
 * @fn void GestionEnv_supprimerBalles()
 * @brief Fonction qui supprime toutes les balles
 * présentent dans l'environnement
 */
void GestionEnv_supprimerBalles(){
	_Balle_foreach(Balle_supprimer);
	//free(balles);
	 g_ptr_array_free(balles, 0);
}

/**
 * @fn int GestionEnv_ballesImmobiles()
 * @brief Fonction qui retourne 1 si toutes les balles 
 * de l'environnement sont immobiles (@see Balle_estImmobile), 0 sinon
 */
int GestionEnv_ballesImmobiles(){

	int ballesImmobiles = 1, i = 0;
	
	while(ballesImmobiles && i < nbBallesCrees){
		ballesImmobiles = Balle_estImmobile(g_ptr_array_index(balles, i++));
	}	

	return ballesImmobiles;	
}

//-------------------------------------------------------------------------------------------------------------
//								Gestion tracer ligne & récupération des caractères
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2)
 * @brief Fonction qui affiche une ligne de (x1,y1) à (x2,y2) et renvoie
 * la chaine de caractères formée par les lettres des balles traversés
 * par cette ligne
 *
 * @param x1 Abscisse du point de départ de la ligne
 * @param y1 Ordonnée du point de départ de la ligne
 * @param x2 Abscisse du point final de la ligne
 * @param y2 Ordonnée du point final de la ligne
 *
 * @return Chaîne de caractère formée par les lettres des balles 
 * traversés par la ligne
 */
typedef int (*FonctionComparer)(const void *, const void *);

#define DROITE -1
#define GAUCHE 1

static int sensLigne = GAUCHE;
static int _comparerBalles(const Balle** a, const Balle** b){
	return ((sensLigne == DROITE) ?  (*a)->cx -  (*b)->cx :  (*b)->cx -  (*a)->cx);
}

char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2){

	// Dessine la ligne
	lineColor(ecran, x1, y1, x2, y2, 0x000000FF);

	// Cherche les balles touchées
	// 80 = diamètre max d'une balle (voir _creerUneBalle)
	const unsigned int nbBallesMaxDiag = sqrt(pow((HAUTEUR_ECRAN-4*OFFSET),2) + 
										      pow((LARGUEUR_ECRAN-2*OFFSET),2))/80;

	unsigned int nbBallesTouches = 0;	
	Balle** ballesTouches = calloc(nbBallesMaxDiag, sizeof(Balle*));
	
	for(unsigned int i = 0; i < nbBallesCrees; i++){
		
		// Vérifie si la balle est touchée par la ligne
		int touche =  cpShapeSegmentQuery(
						   ((Balle*)g_ptr_array_index(balles, i))->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		// Sauvegarde la balle touchée
		if(touche){
			ballesTouches[nbBallesTouches] = (Balle*)g_ptr_array_remove_index(balles, i);
			nbBallesTouches++;
			nbBallesCrees--;
			i--;
		}
	}

	// Met les balles dans l'ordre suivant le tracé de la ligne
	// pour avoir les caractères dans le bon ordre (pour version 1 & 2 de l'algo de recherche)
	sensLigne = ((x1 < x2) ? DROITE : GAUCHE);
	qsort(ballesTouches, nbBallesTouches, sizeof(Balle*), (FonctionComparer) _comparerBalles);

	// Construit la chaine de caractères
	char* lettres = calloc(nbBallesTouches + 1, sizeof(char));
	for(unsigned int i = 0; i < nbBallesTouches; i++){
		char l [2] = { ballesTouches[i]->lettre, 0 }; // Ajoute zéro terminal
		strcat(lettres, l);
	}

	// Supprime les balles touchées du tableau global
	for(unsigned int i = 0; i < nbBallesTouches; i++){
		Balle_supprimer(ballesTouches[i]);	
	}
	free(ballesTouches);

	// Met à jour l'affichage
	SDL_Flip(ecran);

	return lettres;
}

void GestionEnv_afficherMessage(char* message, int x, int y, int taille){

	SDL_Color couleur = {20, 50, 50};
	TTF_Font* police = TTF_OpenFont("arial.ttf", taille);
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);

	SDL_Surface* texte = TTF_RenderText_Blended(police, message, couleur);
	SDL_Rect position = {x, y};

	SDL_BlitSurface(texte, NULL, ecran, &position);
	TTF_CloseFont(police);

	SDL_Flip(ecran);
}

void GestionEnv_viderZoneMessage(){
	boxColor(ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, COULEUR_FOND);
	boxColor(ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, 0x74C0F177);
	SDL_Flip(ecran);
}

//-------------------------------------------------------------------------------------------------------------
//											Gestion de l'aléatoire
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn static unsigned int _randMinMax(int min, int max)
 * @brief Fonction qui retourne un nombre aléatoire 
 * compris entre un minimum et un maximum
 *
 * @param min Valeur minimale de la valeur à renvoyer
 * @param max Valeur maximale de la valeur à renvoyer
 *
 * @return Valeur aléatoire comprise entre min et max
 */
static unsigned int _randMinMax(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

/**
 * @fn static cpVect _randDirection()
 * @brief Fonction qui retourne une direction aléatoire
 * comprise entre (0,-80) et (0,80)
 *
 * @return Direction aléatoire sous forme de vecteur
 */
static cpVect _randDirection(){
	int x = _randMinMax(0,80);
	int dirGauche = _randMinMax(0,1);

	if(dirGauche){
		return cpv(-x,0);
	}
	else {
		return cpv(x,0);
	}
}


/**
 * @var static Uint32 couleurs []
 * @brief Tableau de couleurs codées en RRGGBBAA
 *
 * Utilisé par @see _randCouleur
 * voir http://fr.wikipedia.org/wiki/Liste_de_couleurs
 * pour plus de couleurs
 */ 
static Uint32 couleurs [] = { 
							   0xE67E30FF, /* Abricot */
							   0x74C0F1FF, /* Azur clair */
							   0xFFE436FF, /* Jaune Impérial */
							   0xD2CAECFF, /* Gris de lin */
							   0xC60800FF, /* Coquelicot */
							   0x26619CFF, /* Lapis-lazulli */
							   0x3A9D23FF  /* Vert gazon */
						    };

/**
 * @fn static Uint32 _randCouleur()
 * @brief Fonction qui retourne couleur aléatoire
 * prise dans le tableau couleurs
 *
 * @see couleurs
 *
 * @return Couleur aléatoire sous la forme RRGGBBAA
 */
static Uint32 _randCouleur(){
	return couleurs[_randMinMax(0,6)];
}

/**
 * @var static const int lettres []
 * @brief Tableau de probabilités d'apparition
 * d'un caractère le dictionnaire
 *
 * L'indice correspond à la position de la lettre 
 * dans l'alphabet. Les probabilités sont ajoutés.
 *
 */
#define TAILLE_TAB_LETTRES 26
#define RATIO_VOYELLES 43.41

static const float lettres [] = {
									 9.78,
									11.16,
									14.53,
									16.87,
									31.56,
									32.89,
									34.47,
									35.62,
									45.13,
									45.31,
									45.36,
									49.35,
									51.88,
									59.32,
									65.18,
									67.51,
									68.01,
									76.69,
									86.92,
									93.80,
									97.36,
									98.31,
									98.32,
									98.57,
									98.91,
								   100.00,
							    };

/**
 * @fn static char _randLettre()
 * @brief Fonction qui retourne une lettre majuscule
 * de l'alphabet aléatoirement en suivant les probabilités
 * d'apparition listés dans le tableau lettres
 *
 * @see lettres
 *
 * @return Couleur aléatoire sous la forme RRGGBBAA
 */
static char _randLettre(){
	
	/**
	 * Nombre total de lettres générées
	 */
	static unsigned int lettresTotal = 0;

	/**
	 * Nombre de fois que chaque lettre de l'alphabet
     * a été générée
	 *
	 * nb_Lettres permet d'accéder aux statistique comme suit :
	 *   	nbLettre['A'] => nbLettre['Z']
	 */
	static unsigned int _lettresGeneres[26] = {0};
	static unsigned int* lettresGeneres = _lettresGeneres - 65;

	// Génération d'un nombre réél aléatoire entre 0 et 100
	const float proba = (rand()%10000)/100.0;

	// Cherche la lettre à renvoyer en fonction 
    // du nombre généré
	char lettre = 0;

	unsigned int i = 0, l = 0;
	while(lettre == 0 && i < TAILLE_TAB_LETTRES){	
		l = 65 + i;
		if(proba <= lettres[i++]){
			lettre = (char)l;	
			lettresGeneres[l]++;
		}
	}	

	// La proportion de la lettre trouvée par rapport aux nombre
	// de lettres générés est trop grande
	// Ou si le ratio voyelles/consonnes n'est pas correct (à 10% près)
    // ==> il faut générer un nouveau caractère
	float ratioVoyelles = (lettresGeneres['A'] + lettresGeneres['E'] 
							+ lettresGeneres['I'] +lettresGeneres['O'] + lettresGeneres['U']) / (float)lettresTotal;

	if(lettresTotal > 10 && lettresGeneres[l] > 2){
		if(((lettresGeneres[l]/lettresTotal) > lettres[l]) 
	   		|| ratioVoyelles < RATIO_VOYELLES)
		{
			lettresGeneres[l]--;
			return _randLettre();	
		}
	}
	lettresTotal++;

	return lettre;
}

static cpVect _randPosition(int rayon){

	static char p = 1;	
	p ^= 0x01;

	const unsigned int max = LARGUEUR_ECRAN - 2*OFFSET - rayon;

	if(p){ // 1 fois sur 2 dans la partie gauche du panier
		return cpv(_randMinMax(OFFSET + rayon, max/2), -rayon);
	}
	else { // partie droite du panier
		return cpv(_randMinMax(max/2, max), -rayon);
	}

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
cpShape** donnerSol(){
	return panier;
}

