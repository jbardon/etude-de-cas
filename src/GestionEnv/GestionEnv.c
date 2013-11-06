#include "GestionEnv.h"

//-------------------------------------------------------------------------------------------------------------
//										Déclaration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static unsigned int _randMinMax(int min, int max);
static void _Balle_foreach(Balle_Fonction fonction);
static void _creerUneBalle();
static Uint32 _randCouleur();
static cpVect _randDirection();

//-------------------------------------------------------------------------------------------------------------
//										Variables liées à l'environnement
//-------------------------------------------------------------------------------------------------------------

static cpSpace* espace = NULL;
static cpShape* panier[3]; // Contient les 2 murs et le sol du panier
static double temps = 0;   // Temps qui s'écoule dans l'espace de jeu (gestion avec chipmunk, voir GestionEnv_evoluer)

static SDL_Surface* ecran = NULL;

static Balle** balles = NULL;
static int nbBallesTotal = 0;  // Taille de de la variables du tableau de balles
static int nbBallesCrees = 0;  // Nombre de balles déjà crées
static int timerLancement = 0; // Timer qui sert créer des balles à un intervalle de temps régulier

//-------------------------------------------------------------------------------------------------------------
//										Initialisation de l'environnement
//-------------------------------------------------------------------------------------------------------------
cpSpace* GestionEnv_initChipmunk(){

	srand(time(NULL));	/* A VOIR */
	espace = cpSpaceNew();

	if(espace){
		cpVect gravite = cpv(0, -100);  
		cpSpaceSetGravity(espace, gravite);
	}

	return espace;
}

SDL_Surface* GestionEnv_initSDL(){

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);

	if(ecran){
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
		SDL_WM_SetCaption("Projet étude de cas", NULL);
	}

	return ecran;	
}

//-------------------------------------------------------------------------------------------------------------
//										Suppression de l'environnement
//-------------------------------------------------------------------------------------------------------------
void GestionEnv_quitChipmunk(){
	cpSpaceFree(espace);
}

void GestionEnv_quitSDL(){
	SDL_FreeSurface(ecran);
	TTF_Quit();
	SDL_Quit();
}

//-------------------------------------------------------------------------------------------------------------
//									Evolution de l'environnement (dans le temps)
//-------------------------------------------------------------------------------------------------------------
void GestionEnv_evoluer(){

	// Fait évoluer les balles, applique le déplacement et maj de l'affichage
	cpSpaceStep(espace, uniteTemps);	
	_Balle_foreach(Balle_deplacer);  
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
void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf){
 
	int x = OFFSET;
	int y = OFFSET;
	int l = LARGUEUR_ECRAN - 2*OFFSET;
	int h = 0;
	int e = 10; // Epaisseur du trait (SDL)

	// Création du sol qui est un élément statique
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], FRICTION);
	cpShapeSetElasticity(panier[0], REBOND);
	cpSpaceAddShape(espace, panier[0]);

	y = HAUTEUR_ECRAN - y + e/2;
	thickLineColor(surf, x - e, y, x + l + e, y, e, 0x000000FF);

	// Création du mur gauche
	x = OFFSET;
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1], FRICTION);
	cpShapeSetElasticity(panier[1], REBOND);
	cpSpaceAddShape(espace, panier[1]);

	x = OFFSET - e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 2 * OFFSET;
	thickLineColor(surf, x, y + e, x, h, e, 0x00000FF);

	// Création du mur droit
	x = LARGUEUR_ECRAN - OFFSET;
	panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[2], FRICTION);
	cpShapeSetElasticity(panier[2], REBOND);
	cpSpaceAddShape(espace, panier[2]);

	x = x + e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 2 * OFFSET;
	thickLineColor(surf, x, y + e, x, h, e, 0x000000FF);

}

void GestionEnv_supprimerPanier(){
	for(int i = 0; i < 3; i++){
		cpShapeFree(panier[i]);
	}
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion des balles
//-------------------------------------------------------------------------------------------------------------

// Exécute la fonction donnée en paramètre pour toutes les balles
static void _Balle_foreach(Balle_Fonction fonction){
	for(unsigned int i = 0; i < nbBallesCrees; i++){
		fonction(balles[i]);
	}
}

// Créé une balle dans l'environnement
static void _creerUneBalle(){

	if(nbBallesCrees < nbBallesTotal){

		// Caractéritiques aléatoires pour les balles
		const int rayon = _randMinMax(30,50);
		const cpVect centre = cpv(_randMinMax(OFFSET + rayon, LARGUEUR_ECRAN - 2*OFFSET - rayon), -rayon);
		const cpVect direction = _randDirection();
		const Uint32 couleur = _randCouleur();
		const char lettre = (char) _randMinMax(65,90);

		// Création de la balle
		balles[nbBallesCrees] = Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre);
		nbBallesCrees++;
	}			
}

// Créé une balle et configuration pour créé les n-1 autres balles
// à intervalle de temps donnée (voir config.h)
void GestionEnv_creerBalles(int nbBalles){

	balles = calloc(nbBalles, sizeof(Balle*));

	if(balles){
		nbBallesTotal = nbBalles;	
		nbBallesCrees = 0;	
		_creerUneBalle();		
	}
}

// Supprime toutes les balles créés
void GestionEnv_supprimerBalles(){
	_Balle_foreach(Balle_supprimer);
	free(balles);
}

//-------------------------------------------------------------------------------------------------------------
//								Gestion tracer ligne & récupération des caractères
//-------------------------------------------------------------------------------------------------------------
char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2){

	// Dessine la ligne
	thickLineColor(ecran, x1, y1, x2, y2, 5, 0x000000FF);

	// Cherche les lettres
	char* lettres = calloc(nbBallesCrees + 1, sizeof(char));
	
	for(unsigned int i = 0; i < nbBallesCrees; i++){
		
		// Vérifie si la balle est touchée par la ligne
		int touche =  cpShapeSegmentQuery(
						   balles[i]->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		// La balle est bien touchée
		if(touche){
			strcat(lettres, &(balles[i]->lettre));
		}
	}

	// Met à jour l'affichage
	SDL_Flip(ecran);

	return lettres;
}

//-------------------------------------------------------------------------------------------------------------
//											Gestion de l'aléatoire
//-------------------------------------------------------------------------------------------------------------

// Retourne un nombre aléatoire entre min et max
static unsigned int _randMinMax(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

// Calcule une direction aléatoire
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


// Retourne une couleur (en RRGGBBAA) aléatoires
// voir http://fr.wikipedia.org/wiki/Liste_de_couleurs
static Uint32 couleurs [] = { 
							   0xE67E30FF, /* Abricot */
							   0x74C0F1FF, /* Azur clair */
							   0xFFE436FF, /* Jaune Impérial */
							   0xD2CAECFF, /* Gris de lin */
							   0xC60800FF, /* Coquelicot */
							   0x26619CFF, /* Lapis-lazulli */
							   0x3A9D23FF  /* Vert gazon */
						    };

static Uint32 _randCouleur(){
	return couleurs[_randMinMax(0,6)];
}

//-------------------------------------------------------------------------------------------------------------
//						Fonctions debug (laisser pour compiler testDroiteGauche & testGaucheDroite)
//-------------------------------------------------------------------------------------------------------------
cpShape** donnerSol(){
	return panier;
}

