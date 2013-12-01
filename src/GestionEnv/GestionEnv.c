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
static void _dessinerPanier();

static void _Balle_foreach(Balle_Fonction fonction);
static void _creerUneBalle();

static unsigned int _randMinMax(int min, int max);
static Uint32 _randCouleur();
static cpVect _randDirection();
static char __randLettre(int nbRecursions);
static char _randLettre();
static cpVect _randPosition(int rayon);

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
 * @var GSList balles
 * @brief Tableau des balles pr�sentent dans l'environnement
 * Balles cr��es avec la fonction _creerUneBalle
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
//	GestionEnv_effacerPanier();	
	_Balle_foreach(Balle_deplacer); 
	_dessinerPanier(); 
	SDL_Flip(ecran);

	// Cr�� une balle � chaque intervalle de temps, max = nombre de balles donn�e dans GestionEnv_creerBalles
	if(nbBallesCrees < nbBallesTotal && timerLancement >= DELAI_APPARITION){
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
 
	int x = OFFSET + 2;
	int y = OFFSET;

cpBodySetMoment(espace->staticBody, INFINITY);
cpBodySetMass(espace->staticBody, INFINITY);
cpBodySetForce(espace->staticBody, cpv(1,1));

	// Cr�ation du sol qui est un �l�ment statique
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], FRICTION);
	cpShapeSetElasticity(panier[0], REBOND);
	cpSpaceAddShape(espace, panier[0]);

	// Cr�ation du mur gauche
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1], FRICTION);
	cpShapeSetElasticity(panier[1], REBOND);
	cpSpaceAddShape(espace, panier[1]);

	// Cr�ation du mur droit
	x = LARGUEUR_ECRAN - OFFSET - 2;
	panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[2], FRICTION);
	cpShapeSetElasticity(panier[2], REBOND);
	cpSpaceAddShape(espace, panier[2]);

	// Dessine le panier sur l'�cran
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
	boxColor(ecran, 0, 2*OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN, COULEUR_FOND);
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
	/*for(unsigned int i = 0; i < nbBallesCrees; i++){
		fonction(g_ptr_array_index(balles, i));
	}*/
	g_ptr_array_foreach(balles, (GFunc)fonction, NULL);
}

/**
 * @fn static void _creerUneBalle()
 * @brief Fonction qui cr�� une balle dans l'environnement 
 * avec des param�tres al�atoires :
 *
 * @see Balle_creer
 * rayon: 28 -> 36
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
		const int rayon = _randMinMax(28,36);
		const cpVect centre = _randPosition(rayon);
		const cpVect direction = _randDirection();
		const Uint32 couleur = _randCouleur();
		const char lettre = _randLettre();

		// Cr�ation de la balle
		//balles[nbBallesCrees] = Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre);
		g_ptr_array_add(balles, Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre));
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
 * pr�sentent dans l'environnement
 */
void GestionEnv_supprimerBalles(){
	_Balle_foreach(Balle_supprimer);
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

	// Cherche les balles touch�es
	// 80 = diam�tre max d'une balle (voir _creerUneBalle)
	const unsigned int nbBallesMaxDiag = ceil(sqrt(pow((HAUTEUR_ECRAN-4*OFFSET),2) + 
										      pow((LARGUEUR_ECRAN-2*OFFSET),2))/80) + 1;

	unsigned int nbBallesTouches = 0;	
	Balle** ballesTouches = calloc(nbBallesMaxDiag, sizeof(Balle*));	

	for(unsigned int i = 0; i < nbBallesCrees; i++){
		
		// V�rifie si la balle est touch�e par la ligne
		int touche =  cpShapeSegmentQuery(
						   ((Balle*)g_ptr_array_index(balles, i))->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		// Sauvegarde la balle touch�e
		if(touche){
			ballesTouches[nbBallesTouches] = (Balle*)g_ptr_array_index(balles, i);
			nbBallesTouches++;
		}
	}

	// Met les balles dans l'ordre suivant le trac� de la ligne
	// pour avoir les caract�res dans le bon ordre (pour version 1 & 2 de l'algo de recherche)
	sensLigne = ((x1 < x2) ? DROITE : GAUCHE);
	qsort(ballesTouches, nbBallesTouches, sizeof(Balle*), (FonctionComparer) _comparerBalles);

	// Construit la chaine de caract�res
	char* lettres = (char*) calloc(nbBallesTouches + 1, sizeof(char));

	for(unsigned int i = 0; i < nbBallesTouches; i++){
		char l [2] = { ballesTouches[i]->lettre, '\0' }; // Ajoute z�ro terminal
		strcat(lettres, l);
	}

	// Supprime les balles touch�es du tableau global
	for(unsigned int i = 0; i < nbBallesTouches; i++){
		g_ptr_array_remove(balles, ballesTouches[i]);
		Balle_supprimer(ballesTouches[i]);	
	}
	free(ballesTouches);

	// Met � jour l'affichage
	SDL_Flip(ecran);

	return lettres;
}

static int nbMessages = 0;
void GestionEnv_afficherMessage(char* message, int x, int y, int taille){

	if(!nbMessages++){
		boxColor(ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, 0xA9EAFEFF);
	}

	SDL_Color couleur = {20, 50, 50};
	
	TTF_Font* police = TTF_OpenFont("arial.ttf", taille);
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);

	if(x == ALIGN_CENTRE){
		int w = 0;
		TTF_SizeText(police, message, &w, NULL);
		x = (LARGUEUR_ECRAN - w)/2;
	}

	SDL_Surface* texte = TTF_RenderText_Blended(police, message, couleur);
	SDL_Rect position = {x, y};

	SDL_BlitSurface(texte, NULL, ecran, &position);
	SDL_Flip(ecran);

}

void GestionEnv_viderZoneMessage(){
	nbMessages = 0;
	boxColor(ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, COULEUR_FOND);
	SDL_Flip(ecran);
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
 * prise dans le tableau couleurs
 *
 * @see couleurs
 *
 * @return Couleur al�atoire sous la forme RRGGBBAA
 */
static Uint32 _randCouleur(){
	return couleurs[_randMinMax(0,6)];
}

/**
 * @var static const int lettres []
 * @brief Tableau de probabilit�s d'apparition
 * d'un caract�re le dictionnaire
 *
 * L'indice correspond � la position de la lettre 
 * dans l'alphabet. Les probabilit�s sont ajout�s.
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
 * de l'alphabet al�atoirement en suivant les probabilit�s
 * d'apparition list�s dans le tableau lettres
 *
 * @see lettres
 *
 * @return Couleur al�atoire sous la forme RRGGBBAA
 */
static char _randLettre(){
	return __randLettre(0);
}

static char __randLettre(int nbRecursions){
	
	/**
	 * Nombre total de lettres g�n�r�es
	 */
	static unsigned int lettresTotal = 0;

	/**
	 * Nombre de fois que chaque lettre de l'alphabet
     * a �t� g�n�r�e
	 *
	 * nb_Lettres permet d'acc�der aux statistique comme suit :
	 *   	nbLettre['A'] => nbLettre['Z']
	 */
	static unsigned int _lettresGeneres[26] = {0};
	static unsigned int* lettresGeneres = _lettresGeneres - 65;

	// G�n�ration d'un nombre r��l al�atoire entre 0 et 100
	const float proba = (rand()%10000)/100.0;

	// Cherche la lettre � renvoyer en fonction 
    // du nombre g�n�r�
	char lettre = 0;

	unsigned int i = 0, l = 0;
	while(lettre == 0 && i < TAILLE_TAB_LETTRES){	
		l = 65 + i;
		if(proba <= lettres[i++]){
			lettre = (char)l;	
			lettresGeneres[l]++;
		}
	}	

	// La proportion de la lettre trouv�e par rapport aux nombre
	// de lettres g�n�r�s est trop grande
	// Ou si le ratio voyelles/consonnes n'est pas correct (� 10% pr�s)
    // ==> il faut g�n�rer un nouveau caract�re
	float ratioVoyelles = (lettresGeneres['A'] + lettresGeneres['E'] 
							+ lettresGeneres['I'] +lettresGeneres['O'] + lettresGeneres['U']) / (float)lettresTotal;

	if(nbRecursions < 5 && lettresTotal > 10 && lettresGeneres[l] > 2){
		if(((lettresGeneres[l]/lettresTotal) > lettres[l]) 
	   		|| ratioVoyelles < RATIO_VOYELLES)
		{
			lettresGeneres[l]--;
			return __randLettre(++nbRecursions);	
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

