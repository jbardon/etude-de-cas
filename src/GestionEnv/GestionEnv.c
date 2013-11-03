#include "GestionEnv.h"

//-------------------------------------------------------------------------------------------------------------
//										D�claration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static unsigned int _randMinMax(int min, int max);
static void _Balle_foreach(Balle_Fonction fonction);
static void _creerUneBalle();

//-------------------------------------------------------------------------------------------------------------
//										Variables li�es � l'environnement
//-------------------------------------------------------------------------------------------------------------

static cpSpace* espace = NULL;
static cpShape* panier[3]; // Contient les 2 murs et le sol du panier
static double temps = 0;   // Temps qui s'�coule dans l'espace de jeu (gestion avec chipmunk, voir GestionEnv_evoluer)

static SDL_Surface* ecran = NULL;

static Balle** balles = NULL;
static int nbBallesTotal = 0;  // Taille de de la variables du tableau de balles
static int nbBallesCrees = 0;  // Nombre de balles d�j� cr�es
static int timerLancement = 0; // Timer qui sert cr�er des balles � un intervalle de temps r�gulier

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
	ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);

	if(ecran){
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
		SDL_WM_SetCaption("Projet �tude de cas", NULL);
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
	SDL_Quit();
}

//-------------------------------------------------------------------------------------------------------------
//									Evolution de l'environnement (dans le temps)
//-------------------------------------------------------------------------------------------------------------
void GestionEnv_evoluer(){

	// Fait �voluer les balles, applique le d�placement et maj de l'affichage
	cpSpaceStep(espace, uniteTemps);	
	_Balle_foreach(Balle_deplacer);  
	SDL_Flip(ecran);

	// Cr�� une balle � chaque intervalle de temps, max = nombre de balles donn�e dans GestionEnv_creerBalles
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

	//Cr�ation du sol qui est un �l�ment statique
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], FRICTION);
	cpShapeSetElasticity(panier[0], REBOND);
	cpSpaceAddShape(espace, panier[0]);

	y = HAUTEUR_ECRAN - y + e/2;
	thickLineColor(surf, x - e, y, x + l + e, y, e, 0x000000FF);

	//Cr�ation du mur gauche
	x = OFFSET;
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1], FRICTION);
	cpShapeSetElasticity(panier[1], REBOND);
	cpSpaceAddShape(espace, panier[1]);

	x = OFFSET - e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 2 * OFFSET;
	thickLineColor(surf, x, y + e, x, h, e, 0x00000FF);

	//Cr�ation du mur droit
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

// Ex�cute la fonction donn�e en param�tre pour toutes les balles
static void _Balle_foreach(Balle_Fonction fonction){
	for(unsigned int i = 0; i < nbBallesCrees; i++){
		fonction(balles[i]);
	}
}

// Cr�� une balle dans l'environnement
static void _creerUneBalle(){

	if(nbBallesCrees < nbBallesTotal){

		//Caract�ritiques al�atoires pour les balles
		const int rayon = _randMinMax(30,50);
		const cpVect centre = cpv(_randMinMax(OFFSET + rayon, LARGUEUR_ECRAN - 2*OFFSET - rayon), -rayon);
		const cpVect direction = cpv(20,0);
		const char lettre = (char) _randMinMax(65,90);

		//Cr�ation de la balle
		balles[nbBallesCrees] = Balle_creer(ecran, espace, centre, direction, rayon, 0xFF0000FF, lettre);
		nbBallesCrees++;
	}			
}

// Cr�� une balle et configuration pour cr�� les n-1 autres balles
// � intervalle de temps donn�e (voir config.h)
void GestionEnv_creerBalles(int nbBalles){

	balles = calloc(nbBalles, sizeof(Balle*));

	if(balles){
		nbBallesTotal = nbBalles;	
		nbBallesCrees = 0;	
		_creerUneBalle();		
	}
}

//Supprime toutes les balles cr��s
void GestionEnv_supprimerBalles(){
	_Balle_foreach(Balle_supprimer);
}

//-------------------------------------------------------------------------------------------------------------
//											Gestion de l'al�atoire
//-------------------------------------------------------------------------------------------------------------

//Retourne un nombre al�atoire entre min et max
static unsigned int _randMinMax(int min, int max){
	return (rand() % (max-min)) + min;
}

//Calcule une direction al�atoire
/*
static cpVect* directions = calloc(5, sizeof(cpVect));
directions[0] = cpv(-20,-40); directions[1] = cpv(-40,-40); directions[2] = cpvzero;
directions[3] = cpv(40,-40); directions[4] = cpv(20,40);

static cpVect _randDirection(){
	return directions[_randMinMax(0, sizeof(directions)/sizeof(cpVect))];
}
*/

//-------------------------------------------------------------------------------------------------------------
//						Fonctions debug (laisser pour compiler testDroiteGauche & testGaucheDroite)
//-------------------------------------------------------------------------------------------------------------
cpShape** donnerSol(){
	return panier;
}

