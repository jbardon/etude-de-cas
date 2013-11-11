/**
 * @file GestionEnv.c
 * @brief Gestion de l'environnement graphique et physique du jeu
 * @author J�r�my.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Biblioth�que pour la gestion de la SDL, chipmunk, des balles et
 * du trac� des lignes pour s�lectionner des balles
 *
 */

#include "GestionEnv.h"

//-------------------------------------------------------------------------------------------------------------
//										D�claration des fonctions locales
//-------------------------------------------------------------------------------------------------------------

static unsigned int _randMinMax(int min, int max);
static void _Balle_foreach(Balle_Fonction fonction);
static void _creerUneBalle();
static Uint32 _randCouleur();
static cpVect _randDirection();

//-------------------------------------------------------------------------------------------------------------
//										Variables li�es � l'environnement
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
 * Temps qui s'�coule dans le monde chipmunk � 
 * chaque appel de la fonction GestionEnv_evoluer
 *
 * @see GestionEnv_evoluer
 *
 */ 
static double temps = 0;

/**
 * @var SDL_Surface* ecran
 * @brief Ecran principal de fen�tre SDL
 *
 * @see GestionEnv_initSDL
 * @see GestionEnv_quitSDL
 *
 */ 
static SDL_Surface* ecran = NULL;

/**
 * @var Balle** balles
 * @brief Tableau des balles pr�sentent dans l'environnement
 * Balles cr��es avec la fonction _creerUneBalle
 *
 * @see GestionEnv_creerBalles
 * @see _creerUneBalle
 *
 *
 */ 
static Balle** balles = NULL;

/**
 * @var int nbBallesTotal
 * @brief Taille du tableau de balles
 *
 * @see balles
 */ 
static int nbBallesTotal = 0;  

/**
 * @var int nbBallesCrees
 * @brief Nombre de balles r��lement cr��es dans l'environnement
 *
 * @see GestionEnv_creerBalles
 */ 
static int nbBallesCrees = 0; 

/**
 * @var int timerLancement
 * @brief Compteur de temps pour lancer les balles
 *
 * Le temps �volue avec GestionEnv_evoluer � travers
 * la variable temps. Une fois le compteur � son maximum (DELAI_APPARITION)
 * une balle est cr��e et le timer est remis � z�ro
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
 * @return L'espace physique cr��
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
 * @brief Fonction qui initialise la biblioth�que SDL 
 * et la fen�tre de jeu SDL
 * 
 * @see ecran
 *
 * @return La fen�tre SDL
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
 * @brief Fonction qui d�truit le monde chipmunk
 * 
 * @see GestionEnv_initChipmunk
 * @see espace
 */
void GestionEnv_quitChipmunk(){
	cpSpaceFree(espace);
}

/**
 * @fn Svoid GestionEnv_quitSDL()
 * @brief Fonction qui d�truit la fen�tre de jeu SDL
 * et quitte la biblioth�que SDL 
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
 * qui se d�placent
 */
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

/**
 * @fn void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf)
 * @brief Fonction qui cr�� le panier au sens physique (chipmunk)
 * et affiche ces contours sur la fen�tre SDL
 *
 * @param espace Espace physique chipmunk @see espace
 * @param surf Ecran principal de la fen�tre SDL @see ecran
 */
void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf){
 
	int x = OFFSET;
	int y = OFFSET;
	int l = LARGUEUR_ECRAN - 2*OFFSET;
	int h = 0;
	int e = 10; // Epaisseur du trait (SDL)

cpBodySetMoment(espace->staticBody, INFINITY);
cpBodySetMass(espace->staticBody, INFINITY);
cpBodySetForce(espace->staticBody, cpv(1,1));

	// Cr�ation du sol qui est un �l�ment statique
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], FRICTION);
	cpShapeSetElasticity(panier[0], REBOND);
	cpSpaceAddShape(espace, panier[0]);

	y = HAUTEUR_ECRAN - y + e/2;
	thickLineColor(surf, x - e, y, x + l + e, y, e, 0x000000FF);

	// Cr�ation du mur gauche
	x = OFFSET;
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1], FRICTION);
	cpShapeSetElasticity(panier[1], REBOND);
	cpSpaceAddShape(espace, panier[1]);

	x = OFFSET - e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 2 * OFFSET;
	thickLineColor(surf, x, y + e, x, h, e, 0x00000FF);

	// Cr�ation du mur droit
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

/**
 * @fn void GestionEnv_supprimerPanier()
 * @brief Fonction qui supprime le panier au sens physique (chipmunk)
 */
void GestionEnv_supprimerPanier(){
	for(int i = 0; i < 3; i++){
		cpShapeFree(panier[i]);
	}
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion des balles
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn static void _Balle_foreach(Balle_Fonction fonction)
 * @brief Fonction qui applique une fonction sur toutes
 * les balles pr�sentes dans l'environnement
 *
 * @param fonction Fonction provenant de la biblioth�que Balle
 * ayant aucun retour et pour seul param�tre une balle
 */
static void _Balle_foreach(Balle_Fonction fonction){
	for(unsigned int i = 0; i < nbBallesCrees; i++){
		fonction(balles[i]);
	}
}

/**
 * @fn static void _creerUneBalle()
 * @brief Fonction qui cr�� une balle dans l'environnement 
 * avec des param�tres al�atoires :
 *
 * @see Balle_creer
 * rayon: 30 -> 50
 * centre: x (dans les panier), y (juste au dessus de la fen�tre)
 * direction: (0,-80) -> (0,80)
 * couleur: @see couleurs
 * lettre: A -> Z
 *
 * @param fonction Fonction provenant de la biblioth�que Balle
 * ayant aucun retour et pour seul param�tre une balle
 */
static void _creerUneBalle(){

	if(nbBallesCrees < nbBallesTotal){

		// Caract�ritiques al�atoires pour les balles
		const int rayon = _randMinMax(30,50);
		const cpVect centre = cpv(_randMinMax(OFFSET + rayon, LARGUEUR_ECRAN - 2*OFFSET - rayon), -rayon);
		const cpVect direction = _randDirection();
		const Uint32 couleur = _randCouleur();
		const char lettre = (char) _randMinMax(65,90);

		// Cr�ation de la balle
		balles[nbBallesCrees] = Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre);
		nbBallesCrees++;
	}			
}

/**
 * @fn void GestionEnv_creerBalles(int nbBalles)
 * @brief Fonction qui demande � l'environnement de cr�� n balles.
 *
 * A l'appel de cette fonction une seule balle est r��lement cr��e (@see nbBallesCrees)
 * Les suivantes sont cr��s toutes les n secondes (@see timerLancement, 
 * @see DELAI_APPARITION dans config.h) par la fonction @see GestionEnv_evoluer
 *
 * @param fonction Fonction provenant de la biblioth�que Balle
 * ayant aucun retour et pour seul param�tre une balle
 */
void GestionEnv_creerBalles(int nbBalles){

	balles = calloc(nbBalles, sizeof(Balle*));

	if(balles){
		nbBallesTotal = nbBalles;	
		nbBallesCrees = 0;	
		_creerUneBalle();		
	}
}

/**
 * @fn void GestionEnv_supprimerBalles()
 * @brief Fonction qui supprime toutes les balles
 * pr�sentent dans l'environnement
 */
void GestionEnv_supprimerBalles(){
	_Balle_foreach(Balle_supprimer);
	free(balles);
}

/**
 * @fn int GestionEnv_ballesImmobiles()
 * @brief Fonction qui retourne 1 si toutes les balles 
 * de l'environnement sont immobiles (@see Balle_estImmobile), 0 sinon
 */
int GestionEnv_ballesImmobiles(){

	int ballesImmobiles = 1, i = 0;
	
	while(ballesImmobiles && i < nbBallesCrees){
		ballesImmobiles = Balle_estImmobile(balles[i++]);
	}	

	return ballesImmobiles;	
}

//-------------------------------------------------------------------------------------------------------------
//								Gestion tracer ligne & r�cup�ration des caract�res
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2)
 * @brief Fonction qui affiche une ligne de (x1,y1) � (x2,y2) et renvoie
 * la chaine de caract�res form�e par les lettres des balles travers�s
 * par cette ligne
 *
 * @param x1 Abscisse du point de d�part de la ligne
 * @param y1 Ordonn�e du point de d�part de la ligne
 * @param x2 Abscisse du point final de la ligne
 * @param y2 Ordonn�e du point final de la ligne
 *
 * @return Cha�ne de caract�re form�e par les lettres des balles 
 * travers�s par la ligne
 */
char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2){

	// Dessine la ligne
	lineColor(ecran, x1, y1, x2, y2, 0x000000FF);

	// Cherche les lettres
	char* lettres = calloc(nbBallesCrees + 1, sizeof(char));
	
	for(unsigned int i = 0; i < nbBallesCrees; i++){
		
		// V�rifie si la balle est touch�e par la ligne
		int touche =  cpShapeSegmentQuery(
						   balles[i]->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		// La balle est bien touch�e
		if(touche){
			char l [2] = { balles[i]->lettre, 0 }; // Ajoute z�ro terminal
			strcat(lettres, l);
		}
	}

	// Met � jour l'affichage
	SDL_Flip(ecran);

	return lettres;
}

//-------------------------------------------------------------------------------------------------------------
//											Gestion de l'al�atoire
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn static unsigned int _randMinMax(int min, int max)
 * @brief Fonction qui retourne un nombre al�atoire 
 * compris entre un minimum et un maximum
 *
 * @param min Valeur minimale de la valeur � renvoyer
 * @param max Valeur maximale de la valeur � renvoyer
 *
 * @return Valeur al�atoire comprise entre min et max
 */
static unsigned int _randMinMax(int min, int max){
	return (rand() % (max - min + 1)) + min;
}

/**
 * @fn static cpVect _randDirection()
 * @brief Fonction qui retourne une direction al�atoire
 * comprise entre (0,-80) et (0,80)
 *
 * @return Direction al�atoire sous forme de vecteur
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
 * @brief Tableau de couleurs cod�es en RRGGBBAA
 *
 * Utilis� par @see _randCouleur
 * voir http://fr.wikipedia.org/wiki/Liste_de_couleurs
 * pour plus de couleurs
 */ 
static Uint32 couleurs [] = { 
							   0xE67E30FF, /* Abricot */
							   0x74C0F1FF, /* Azur clair */
							   0xFFE436FF, /* Jaune Imp�rial */
							   0xD2CAECFF, /* Gris de lin */
							   0xC60800FF, /* Coquelicot */
							   0x26619CFF, /* Lapis-lazulli */
							   0x3A9D23FF  /* Vert gazon */
						    };

/**
 * @fn static Uint32 _randCouleur()
 * @brief Fonction qui retourne couleur al�atoire
 * prise dans le tableau @see couleurs
 *
 * @return Couleur al�atoire sous la forme RRGGBBAA
 */
static Uint32 _randCouleur(){
	return couleurs[_randMinMax(0,6)];
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

